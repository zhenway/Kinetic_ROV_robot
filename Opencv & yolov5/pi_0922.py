import cv2
import zmq
import base64
 
 
def main():
    '''
    主函数
    '''
    IP = '10.60.132.145' #上位机视频接受端的IP地址
 
    # 创建并设置视频捕获对象
    cap = cv2.VideoCapture(0)
    print("摄像头是否已经打开 ？ {}".format(cap.isOpened()))
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 320)  # 设置图像宽度
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)  # 设置图像高度
 
    # 建立TCP通信协议
    contest = zmq.Context()
    footage_socket = contest.socket(zmq.PAIR)
    footage_socket.connect('tcp://%s:5555'%IP)
 
    while True:
        # 读取图像
        ret, frame = cap.read()
 
        # 转换为流数据并编码
        encoded, buffer = cv2.imencode('.jpg', frame) 
        jpg_as_test = base64.b64encode(buffer) #把内存中的图像流数据进行base64编码
 
        # 发送数据
        footage_socket.send(jpg_as_test) #把编码后的流数据发送给视频的接收端
        cv2.waitKey(5) #延时等待，防止出现窗口无响应
 
 
if __name__ == '__main__':
    '''
    程序入口
    '''
    main()
