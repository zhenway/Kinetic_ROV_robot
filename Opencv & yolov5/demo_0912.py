import cv2
import numpy as np
import torch
from models.common import DetectMultiBackend
from utils.general import check_img_size, non_max_suppression
from utils.plots import Annotator, colors
from utils.torch_utils import select_device
from utils.augmentations import letterbox

if __name__ == '__main__':
    # Load model
    device = select_device('')
    weights = "yolov5s.pt"
    dnn = False
    data = "data/coco128.yaml"
    model = DetectMultiBackend(weights, device=device, dnn=dnn, data=data)
    stride, names, pt, jit, onnx, engine = model.stride, model.names, model.pt, model.jit, model.onnx, model.engine
    imgsz = check_img_size((640, 640), s=stride)  # check image size

    model.warmup()  # warmup

    capture = cv2.VideoCapture(0)
    if not capture.isOpened():
        print("Error: Could not open video capture device.")
        exit()

    while True:
        # 获取一帧
        ret, frame = capture.read()
        if not ret:
            break
        frame = cv2.flip(frame, 1)  # 图像翻转

        # 图像预处理
        img = letterbox(frame, imgsz, stride=stride)[0]  # 调整图像尺寸并保持纵横比
        img = img.transpose((2, 0, 1))[::-1]  # HWC to CHW, BGR to RGB
        img = np.ascontiguousarray(img)

        # 转换为PyTorch张量
        im = torch.from_numpy(img).to(device)
        im = im.float()  # uint8 to fp32
        im /= 255  # 归一化
        if len(im.shape) == 3:
            im = im[None]  # 添加批量维度

        # 模型推理
        pred = model(im, augment=False, visualize=False)
        pred = non_max_suppression(pred)
        det = pred[0]
        annotator = Annotator(frame, line_width=3, example=str(names))
        for *xyxy, conf, cls in det:
            c = int(cls)
            label = names[c]
            annotator.box_label(xyxy, label, color=colors(c, True))

        im0 = annotator.result()
        cv2.imshow('frame', im0)

        # 如果输入q，则退出
        if cv2.waitKey(1) == ord('q'):
            break

    capture.release()
    cv2.destroyAllWindows()