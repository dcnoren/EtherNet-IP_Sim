# EtherNet-IP_Sim

This is a small EtherNet/IP protocol simulator. It has no smarts built-in, and all response it generates are dumb, hard-coded responses. However, it will 'speak' a limited number of valid, EtherNet/IP communications.

This is really only useful for limited testing of tools you write, or to place on the network to see if anything ever communicates with it. That said, please do not run this on anything but a heavily-controlled test environment where you know what exists on the network. There may be buffer overflows and other vulnerabilities in this tool. I have not spent ANY time running fuzzers against it.

## TO-DO:
- Add in additional parsers for various EtherNet/IP commands, as well as more and smarter (=more better!) responses.
