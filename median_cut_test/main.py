#from median_cut import median_cut
import median_cut
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches

def main():
    # Load an image
    image = np.array(Image.open('Thumersbach.png', 'r').convert('RGB'))

    # Draw a box for the region on the image by the given input in form
    # Region 0: a b c d
    # Centroid pos: x,y

    fig, ax = plt.subplots()

    # Display the image
    # print(image.shape)
    # median = median_cut(image, 2)
    #
    #
    # pos = (median % image.shape[1], median // image.shape[1])
    # ax.plot(pos[0],pos[1], 'ro')
    # print(pos)

    image = median_cut.scale_by_cos_phi(image)

    reg_count = int(input())
    for i in range(2**reg_count):
        a, b, c, d = map(int, input().split())
        clr = np.sum(image[b:d, a:c], axis=(0,1))
        clr = clr / np.linalg.norm(clr)
        rect = patches.Rectangle((a,b), (c-a), (d-b), linewidth=1, edgecolor='r', facecolor=clr)
        ax.add_patch(rect)

        a,b = map(int, input().split())
        # set poi t color equal to the sum of pixel values in the region

        ax.add_patch(patches.Circle((a,b), 1, color='r'))

    ax.imshow(image)

    plt.show()

    # # Reduce the number of colors in the image to 16
    # num_colors = 2
    # representative_colors = median_cut(image, num_colors)
    #
    # # Create a new image using the representative colors and approximate every pixel
    # new_image = np.zeros_like(image)
    # for i, color in enumerate(representative_colors):
    #     new_image[np.all(abs(image - color) < (10,10,10), axis=-1)] = color
    #
    #
    #
    # print(representative_colors)
    #
    # # Display the original and new images
    # plt.figure(figsize=(10, 5))
    # plt.subplot(1, 3, 1)
    # plt.imshow(image)
    # plt.title('Original Image')
    # plt.axis('off')
    #
    # plt.subplot(1, 3, 2)
    # #show reduced colors as a grid of color swatches
    # plt.imshow([representative_colors], aspect='auto')
    # plt.title('Reduced Color Image')
    # plt.axis('off')
    #
    # plt.subplot(1, 3, 3)
    # plt.imshow(new_image)
    #
    # plt.show()

if __name__ == '__main__':
    main()