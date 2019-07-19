# 定义状态
1. 连接服务类型的状态用`ConnectXXX`
2. 收发消息类型的状态去掉`CEvent`前缀，视情况而定可以进一步缩短
3. 等待消息类型的状态用`WaitXXX`形式，另外去掉`CEvent`字样
4. 如果一个状态不会有超时事件，那么设置`wait:false`
5. 事件（signals）约定命名为`ok|failed`，其它情况自定