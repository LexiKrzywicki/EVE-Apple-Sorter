import numpy as np
import cv2
import torch
import glob as glob

from model import create_model

import capture

def inference(cap_image):

    # set the computation device
    device = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')
    # load the model and the trained weights
    model = create_model(num_classes=6).to(device)
    # model = create_model(num_classes=3).to(device)
    model.load_state_dict(torch.load(
        '../outputs/model100.pth', map_location=device
    ))
    model.eval()

    # directory where all the images are present
    # DIR_TEST = '../test_data'
    # test_images = glob.glob(f"{DIR_TEST}/*")
    #print(f"Test instances: {len(test_images)}")

    # classes: 0 index is reserved for background
    CLASSES = [
        'background', 'bruise', 'cut', 'puncture', 'stem', 'arm'
    ]

    # define the detection threshold...
    # ... any detection having score below this will be discarded
    detection_threshold = 0.6

    # get the image file name for saving output later on
    #image_name = cap_image.split('\\')[-1].split('.')[0]
    #image = cv2.imread(cap_image)
    #orig_image = cap_image.copy()
    # cv2.imshow("Original", orig_image)
    # cv2.waitKey(0)
    # BGR to RGB
    image = cv2.cvtColor(cap_image, cv2.COLOR_BGR2RGB).astype(np.float32)
    # make the pixel range between 0 and 1
    image /= 255.0
    # bring color channels to front
    image = np.transpose(image, (2, 0, 1)).astype(np.float64)
    # convert to tensor
    image = torch.tensor(image, dtype=torch.float).cuda()
    # add batch dimension
    image = torch.unsqueeze(image, 0)
    with torch.no_grad():
        outputs = model(image)

    # load all detection to CPU for further operations
    outputs = [{k: v.to('cpu') for k, v in t.items()} for t in outputs]

    # carry further only if there are detected boxes
    # if len(outputs['boxes']) != 0:
    boxes = outputs[0]['boxes'].data.numpy()
    scores = outputs[0]['scores'].data.numpy()
    label = outputs[0]['labels'].data.numpy()

    #removes arm and stem from list
    if boxes.size > 0:
        count = 0
        for i in label:
            if i == 5 or i == 4:
                label = np.delete(label, count, axis=0)
                boxes = np.delete(boxes, count, axis=0)
                scores = np.delete(scores, count, axis=0)
            count = count + 1

    # # filter out boxes according to `detection_threshold`
    boxes = boxes[scores >= detection_threshold].astype(np.int32)
    draw_boxes = boxes.copy()
    # get all the predicited class names
    pred_classes = [CLASSES[i] for i in outputs[0]['labels'].cpu().numpy()]

    # draw the bounding boxes and write the class name on top of it
    for j, box in enumerate(draw_boxes):
        cv2.rectangle(cap_image,
                    (int(box[0]), int(box[1])),
                    (int(box[2]), int(box[3])),
                    (0, 0, 255), 2)
        cv2.putText(cap_image, pred_classes[j], 
                    (int(box[0]), int(box[1]-5)),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 
                    2, lineType=cv2.LINE_AA)

    #cv2.imshow('Prediction', cap_image)
    cv2.imwrite('Prediction.png', cap_image)
    #cv2.waitKey(0)

    #print(pred_classes)
    #print(boxes)

    if len(pred_classes) > 0:
        for s in pred_classes:
            if s == 'stem' or s == 'arm':
                pred_classes.remove(s)  #removes stem from list

    #cv2.destroyAllWindows()

    return boxes