import numpy as np
from PIL import Image

IMG_SIZE = None


# Functions
def rescale_image(img):
    global IMG_SIZE
    # Convert to gray-scale and resize it
    image = img.convert('L').resize(IMG_SIZE)
    # Convert image to array and return it
    return image

def preprocess_observation(obs):
    global IMG_SIZE
    # Convert to gray-scale and resize it
    image = Image.fromarray(obs, 'RGB').convert('L').resize(IMG_SIZE)
    # Convert image to array and return it
    return np.asarray(image.getdata(), dtype=np.uint8).reshape(image.size[1],
                                                               image.size[0])


def get_next_state(current, obs):
    # Next state is composed by the last 3 images of the previous state and the
    # new observation
    return np.append(current[1:], [obs], axis=0)
