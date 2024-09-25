import cv2
import zmq
import base64
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

    # Setup ZeroMQ context and socket
    context = zmq.Context()
    footage_socket = context.socket(zmq.PAIR)
    footage_socket.bind('tcp://*:5555')
    cv2.namedWindow('frame', flags=cv2.WINDOW_NORMAL | cv2.WINDOW_KEEPRATIO)

    while True:
        print("Listening...")
        frame = footage_socket.recv_string()  # receive TCP transmitted frame video image data
        img = base64.b64decode(frame)  # base64 decode the data and store it in memory img variable
        npimg = np.frombuffer(img, dtype=np.uint8)  # decode this cache into a one-dimensional array
        source = cv2.imdecode(npimg, cv2.IMREAD_COLOR)  # decode the one-dimensional array into the image source

        # Image preprocessing
        img = letterbox(source, imgsz, stride=stride)[0]  # adjust image size and keep aspect ratio
        img = img.transpose((2, 0, 1))[::-1]  # HWC to CHW, BGR to RGB
        img = np.ascontiguousarray(img)

        # Convert to PyTorch tensor
        im = torch.from_numpy(img).to(device)
        im = im.float()  # uint8 to fp32
        im /= 255  # normalize
        if len(im.shape) == 3:
            im = im[None]  # add batch dimension

        # Model inference
        pred = model(im, augment=False, visualize=False)
        pred = non_max_suppression(pred)
        det = pred[0]
        annotator = Annotator(source, line_width=3, example=str(names))
        for *xyxy, conf, cls in det:
            c = int(cls)
            label = names[c]
            annotator.box_label(xyxy, label, color=colors(c, True))

        im0 = annotator.result()
        cv2.imshow('frame', im0)

        # If input q, then exit
        if cv2.waitKey(1) == ord('q'):
            break

    footage_socket.close()
    cv2.destroyAllWindows()