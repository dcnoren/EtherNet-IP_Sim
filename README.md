# EtherNet-IP_Sim

This is a small EtherNet/IP protocol simulator for Windows. It has no smarts built-in, and all response it generates are dumb, hard-coded responses. However, it will 'speak' a limited number of valid, EtherNet/IP communications.

This is really only useful for limited testing of tools you write, or to place on the network to see if anything ever communicates with it. That said, please do not run this on anything but a heavily-controlled test environment where you know what exists on the network. There may be buffer overflows and other vulnerabilities in this tool. I have not spent ANY time running fuzzers against it.

## Usage
1. Clone this.
2. Navigate to cpp/bin directory with command prompt on Windows.
3. Run enip-sim.exe.
4. Communicate with [ip]:44818. For example, open RSLinx and add a PLC, specifying the IP this tool is running on.
5. Source code is provided in the context of a Visual Studio 2015 Community project folder, located in cpp/src.

## TO-DO:
- Add in additional parsers for various EtherNet/IP commands, as well as more and smarter (=more better!) responses.
- Currently only allows up to 255 data bytes. Spec says up to 65511 may be sent.
- Fix a potential buffer overflow that is only mitigated by accepting only 255 data packets.
- Linux port?
