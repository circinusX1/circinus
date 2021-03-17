include("./modules/_oledfont.inc");

// system("config-pin P9.17 i2c");
// system("config-pin P9.18 i2c");


class Oled96 extends I2C
{
    _oled_flip=false;     // unless they are not initialised they are / class
    _oled_type="";
    _iScreenOffset=0;
    _ucscreen=[];
    _text="";

    // 2, 0x3c
    constructor(i2cbus, address, name)
    {
        // bind vars / instance (not per class)
        println("CALLING CTOR ");
        base.constructor(this, i2cbus, address, name); // on BBB I2C1 P9.17 P9.18
        _iScreenOffset = 0;
        _oled_flip = false;
        _oled_type = "";
        _text = "";
        _ucscreen=array(1024,0);


    }

    function xclose()
    {
        base.iowrite(0x00,[0xae]);
        base.close();
    }

    function xopen(oledtype, set_invert, flip)
    {
        print("call base.open\n");

        if(base.open(0))
        {
            local init;
            _oled_type = oledtype;
            if(oledtype="128x32")
            {
                init=[0xae,0xd5,0x80,0xa8,0x1f,0xd3,0x00,
                      0x40,0x8d,0x14,0xa1,0xc8,0xda,0x02,0x81,
                      0x7f,0xd9,0xf1,0xdb,0x40,0xa4,0xa6,0xaf];
            }
            else
            {
                init=[0xae,0xa8,0x3f,0xd3,0x00,0x40,0xa1,
                      0xc8,0xda,0x12,0x81,0xff,0xa4,0xa6,0xd5,
                      0x80,0x8d,0x14,0xaf,0x20,0x02];
            }
            base.iowrite(0x00, init);
            if(set_invert)
            {
                base.iowrite(0x00,[0xa7]);
            }
            if(flip)
            {
                base.iowrite(0x00,[0xa0]);
                base.iowrite(0x00,[0xc0]);
                _oled_flip=bflip;
            }
            return true;
        }
        return false;
    }

    function opendev(){return base.open(2);}

    function _set_pos(x,y)
    {
        _iScreenOffset = (y*128)+x;
        if (_oled_type != "64x32") // visible display starts at column 32, row 4
        {
            x += 32; // display is centered in VRAM, so this is always true
            if (_oled_flip == 0) // non-flipped display starts from line 4
                y += 4;
        }
        else if (_oled_type == "132x64") // SH1106 has 128 pixels centered in 132
        {
            x += 2;
        }

        base.iowrite(0, [0xb0 | y]);                  // go to page Y
        base.iowrite(0, [0x00 | (x & 0xf)]);          // // lower col addr
        base.iowrite(0, [0x10 | ((x >> 4) & 0xf)]);   // upper col addr
    }

    function _iowrite_block(arr)
    {
        base.iowrite(0x40, arr);
        arrcpyn(_ucscreen, _iScreenOffset, arr);
        _iScreenOffset += arr.len();
    }

    function _set_pixel(x, y, ucColor)
    {
        local uc = [0];
        local ucOld = [0];

        local i = ((y >> 3) * 128) + x;
        if (i < 0 || i > 1023) // off the screen
            return -1;
        uc[0] = ucOld[0] = ucScreen[i];
        uc[0] = uc[0] & ~(0x1 << (y & 7));
        if (ucColor)
        {
            uc[0] = uc[0] | (0x1 << (y & 7));
        }
        if (uc[0] != ucOld[0]) // pixel changed
        {
            _set_pos(x, y>>3);
            _iowrite_block(uc);
        }
        return 0;
    }

    function write_string(x, y, szMsg, iSize)
    {
        local i, iLen;
        local s = array(16);

        _text=szMsg;
//	print("type ================= " + typeof szMsg + "\n");
        iLen = szMsg.len();
        if (iSize >= 3) // draw 16x32 font
        {
            if (iLen+x > 8) iLen = 8-x;
            if (iLen < 0) return -1;
            x *= 16;
            for (i=0; i<iLen; i++)
            {
                s = ucFont.slice(9728 + (szMsg[i] * 64), 9728 + (szMsg[i] * 64)+16);
                _set_pos(x+(i*16), y);
                _iowrite_block(s);

                _set_pos(x+(i*16), y+1);
                s = ucFont.slice(16 + 9728 + (szMsg[i] * 64), 16 + 9728 + (szMsg[i] * 64)+16);
//                _iowrite_block(s+16, 16);
                _iowrite_block(s);
                _set_pos(x+(i*16), y+2);

                s = ucFont.slice(32 + 9728 + (szMsg[i] * 64), 32 + 9728 + (szMsg[i] * 64)+16);
                _iowrite_block(s);
                //			_set_pos(x+(i*16), y+3);
                //			_iowrite_block(s+48, 16);
            }
        }
        else if (iSize >= 2) // draw 8x8 font
        {
            _set_pos(x*8, y);
            if (iLen + x > 16) iLen = 16 - x; // can't display it
            if (iLen < 0)return -1;

            for (i=0; i<iLen; i++)
            {
                s = ucFont.slice(szMsg[i] * 8, szMsg[i] * 8 +  8);
                _iowrite_block(s); 			// iowrite character pattern
            }
        }
        else // 6x8
        {
            _set_pos(x*6, y);
            if (iLen + x > 21) iLen = 21 - x;
            if (iLen < 0) return -1;
            for (i=0; i<iLen; i++)
            {
                s = ucSmallFont.slice(szMsg[i]*6, szMsg[i]*6+6);
                _iowrite_block(s);
            }
        }
        return 0;
    }

    function clear_display(ucData)
    {
        local y;

        local iLines = (_oled_type == "128x32" || _oled_type == "64x32") ? 4:8;
        local iCols = (_oled_type == "64x32") ? 4:8;
        local temp = array(iCols*16, ucData);
        for (y=0; y<iLines; y++)
        {
            _set_pos(0,y); // set to (0,Y)
            _iowrite_block(temp); // fill with data byte
        } // for y

        write_string(0,0,"          ",3);
        write_string(0,1,"          ",3);
        write_string(0,2,"          ",3);
        write_string(0,5,"          ",3);
        return 0;
    }


    //  GOES TO WEB as text:"value" in the OLED JSON
    function get_value(w){
        var json_obj = {
            text = _text,
        }
        return json_obj;
    }

    // http://localhost:8000/<?>OLED96/text=asds:f
    function set_value(key, value){
        if(key=="text")
        {
            println("NEW TEXT:" +  value);
            _text=value;
            write_string(1,3,_text,2);
            return true;
        }
        return false;
    }

    function on_event(thiso, newdata)
    {
        println("EVENT for:" + thiso + " = " + newdata);
    }
};

