/*
Copyright (c) 2014-2016 Marius.C All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
advertising materials, and other materials related to such
distribution and use acknowledge that the software was developed
by the https://github.com/comarius. The name of the
https://github.com/comarius may not be used to endorse or promote
products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifdef WITH_SSH


#include "comssh.h"
#include "logs.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

ComSsh::ComSsh(const char* constr)
{
    std::string  a;
    char chs[2]={0};

    for(int ch = 0; constr[ch] != 0;ch++)
    {
        chs[0] = constr[ch];
        if(ch=='/')
        {
            _user=a;
            a.clear();
            continue;
        }
        if(ch=='@')
        {
            _pass=a;
            a.clear();
            continue;
        }
        if(ch==':')
        {
            _host=a;
            a.clear();
            continue;
        }
        a.append(chs);
    }
    _port = ::atoi(a.c_str());
}

ComSsh::~ComSsh()
{
    iclose();
}

bool    ComSsh::iopen(int )
{
    iclose();

    _ssh_session = ssh_new();
    if(0 == _ssh_session)
        return false;

    int verbosity = 0;//SSH_LOG_PROTOCOL;
    ssh_options_set(_ssh_session, SSH_OPTIONS_HOST, _host.c_str());
    ssh_options_set(_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    ssh_options_set(_ssh_session, SSH_OPTIONS_PORT, &_port);
    ssh_options_set(_ssh_session, SSH_OPTIONS_USER, _user.c_str());

    int rc = ssh_connect(_ssh_session);
    if (rc != SSH_OK)
    {
        LOGW( "Error connecting to <<" << _host << " " << ssh_get_error(_ssh_session));
        return false;
    }
    if (_verify_knownhost() < 0)
    {
        return false;
    }
    LOGD1( "trying to log on with 'p1' password on user:'"<< _user);
    rc = ssh_userauth_password(_ssh_session, NULL, _pass.c_str());
    if (rc != SSH_AUTH_SUCCESS)
    {
        LOGW("Could not connect: : " << _host << " and _user: " << _user );
        //            string password;
        //            cin >> password;
        //            rc = ssh_userauth_password(_ssh_session, NULL, password.c_str());
        //           if (rc != SSH_AUTH_SUCCESS)
        //           {
        //               cout <<RED<< "Error authenticating with password: " << ssh_get_error(_ssh_session) <<CEND<< "\n";
        return false;
        //           }
    }
    return _opentty();
}

bool    ComSsh::_opentty()
{
    assert(_ssh_session);

    _channel = channel_new(_ssh_session);
    if (_channel == NULL)
    {
        return false;
    }
    int rc = channel_open_session(_channel);
    if (rc != SSH_OK)
    {
        return false;
    }

    rc = channel_request_pty(_channel);
    if (rc != SSH_OK)
        return false;
    rc = channel_change_pty_size(_channel, 80, 24);
    if (rc != SSH_OK)
        return false;
    rc = channel_request_shell(_channel);
    if (rc != SSH_OK)
        return false;
    return (channel_is_open(_channel) && !channel_is_eof(_channel));
}

void    ComSsh::iclose()
{
    if(_channel)
    {
        channel_send_eof(_channel);
        sleep(3);
        channel_close(_channel);
        channel_free(_channel);
        _channel= 0;
    }

    if(_ssh_session)
    {
        ssh_disconnect(_ssh_session);
        ssh_free(_ssh_session);
        _ssh_session=0;
    }
}

void    ComSsh::flush()
{
}

size_t     ComSsh::bread(uint8_t* buff, int len, int)
{
    assert(_channel);
    return channel_read_nonblocking(_channel, buff, len, 0);
}

int     ComSsh::bwrite(const uint8_t* buff, int len, int)
{
    assert(_channel);
    size_t bytes = len;
    size_t sent = 0;
    while(bytes > 0)
    {
        int nwritten = channel_write(_channel, (const char*)buff+sent, bytes);
        if(nwritten>0)
        {
            bytes-=nwritten;
            sent+=nwritten;
        }
        else
        {
            LOGW(__PRETTY_FUNCTION__);
            break;
        }
    }
    return sent;
}

int ComSsh::_verify_knownhost()
{
    int state, hlen;
    unsigned char *hash = NULL;
    char *hexa;

    state = ssh_is_server_known(_ssh_session);
    hlen = ssh_get_pubkey_hash(_ssh_session, &hash);
    if (hlen < 0)
        return -1;
    switch (state)
    {
    case SSH_SERVER_KNOWN_OK:
        break; /* ok */
    case SSH_SERVER_KNOWN_CHANGED:
        fprintf(stderr, "Host key for server changed: it is now:\n");
        ssh_print_hexa("Public key hash", hash, hlen);
        fprintf(stderr, "For security reasons, connection will be stopped\n");
        free(hash);
        return -1;
    case SSH_SERVER_FOUND_OTHER:
        fprintf(stderr, "The host key for this server was not found but an other"
                        "type of key exists.\n");
        fprintf(stderr, "An attacker might change the default server key to"
                        "confuse your client into thinking the key does not exist\n");
        free(hash);
        return -1;
    case SSH_SERVER_FILE_NOT_FOUND:
        fprintf(stderr, "Could not find known host file.\n");
        fprintf(stderr, "If you accept the host key here, the file will be"
                        "automatically created.\n");
        /* fallback to SSH_SERVER_NOT_KNOWN behavior */
        __falltrough;
    case SSH_SERVER_NOT_KNOWN:
        hexa = ssh_get_hexa(hash, hlen);
        printf("The server is unknown. Do you trust the host key?\n");
        printf( "Public key hash: %s\n", hexa);
        free(hexa);
        /*
        if (fgets(buf, sizeof(buf), stdin) == NULL)
        {
            free(hash);
            return -1;
        }
        if (strncasecmp(buf, "yes", 3) != 0)
        {
            free(hash);
            return -1;
        }
        */
        if (ssh_write_knownhost(_ssh_session) < 0)
        {
            fprintf(stderr, "Error %s\n", strerror(errno));
            free(hash);
            return -1;
        }
        break;
    case SSH_SERVER_ERROR:
        fprintf(stderr, "Error %s", ssh_get_error(_ssh_session));
        free(hash);
        return -1;
    }
    free(hash);
    return 0;
}
#pragma GCC diagnostic pop


#endif //#ifdef WITH_SSH
