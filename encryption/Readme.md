实现是比较简单的，真正涉及算法的不到一百行，其他的都是些算法本身的置换数组.

其中书上的例子密钥是 0x31, 0x32, ... 0x38, 转换过来就是12345678。但是按照P1置换舍弃掉每个字节的最低位的话，0x31 == 0x32, 0x33 == 0x34...，这样以来就会导致不同的密钥加密相同的密文，生成相同的密文。