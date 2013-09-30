#Simple-Proxy


##Description

proxy的端口是8080，server的端口是8088。

因为考虑到client发送connect请求可能不传数据或者恶意ddos攻击，所以用poll轮询
client的连接请求.

在Proxy到server的连接中直接采用阻塞的方式，因为我知道这是安全的。接下来是
真正的服务器，用python模拟一个.

##Introduction

代理服务器的工作流程就是：把client的访问请求发送到proxy，proxy再把这些数据转到
server，然后server对这个请求产生响应数据，发送到proxy，proxy再把这些数据转到
client。经过了proxy，那就可以做好多事情了，可以自己设置个墙，自己设置访问
优先级，统计流量啥的就不说了，反正可以做各种好玩的。因为刚开始做，做个简单的，
以后可以慢慢加功能。
