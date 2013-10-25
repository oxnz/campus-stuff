# Large Scale Data Process Competition Project

## File Orgnization:

-------\__  input : containts input file for test
       |__  output: output directory for test

TODO: update two lines below:
Latitude:       39.2600000~41.0300000
Longitude:      116.0000000~117.0000000

out file format:


0000000: 0052 0b00 0200 a4e3 13cf 4c12 0000 8aad  .R........L.....
0000010: 0b00 0000 f3e3 13cf 4c12 0000 54c2 0b00  ........L...T...
0000020: 0000 eee3 13cf 4c12 0000 74c2 0b00 0200  ......L...t.....
0000030: abe3 13cf 4c12 0000 b7c3 0b00 0000 aae3  ....L...........
0000040: 13cf 4c12 0000 86c5 0b00 0000 ebe3 13cf  ..L.............
0000050: 4c12 0000 08c7 0b00 0000 bbe3 13cf 4c12  L.............L.
0000060: 0000 15c7 0b00 0000 00e4 13cf 4c12 0000  ............L...
0000070: 81c7 0b00 0000 f8e3 13cf 4c12 0000 82c7  ..........L.....
0000080: 0b00 0200 f1e3 13cf 4c12 0000 9fc7 0b00  ........L.......
0000090: 0000 b2e3 13cf 4c12 0000 dfc7 0b00 0000  ......L.........

rsid: 741888 orecv status: 2time: 20121101001636
rsid: 765322 orecv status: 0time: 20121101001715
rsid: 770644 orecv status: 0time: 20121101001710
rsid: 770676 orecv status: 2time: 20121101001643
rsid: 770999 orecv status: 0time: 20121101001642
rsid: 771462 orecv status: 0time: 20121101001707
rsid: 771848 orecv status: 0time: 20121101001659
rsid: 771861 orecv status: 0time: 20121101001728
rsid: 771969 orecv status: 0time: 20121101001720
rsid: 771970 orecv status: 2time: 20121101001713

first 4 bytes: 0052 0b00 = rsid 741888
second 2 bytes: 0200 0000 0000 0000 = status 2
third 8 bytes: a4e3 13cf 4c12 0000 = time 20121101001636

