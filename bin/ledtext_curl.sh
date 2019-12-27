#!/bin/bash
_pcurl -H "Content-Type: application/json" -X POST -d '{"OLED96":{"text":"new text","refresh":1}}' http://localhost:8000
