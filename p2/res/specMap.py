from PIL import Image, ImageDraw

def create_colored_image(width, height, color):
    # Create a new image with the specified color
    image = Image.new("RGB", (width, height), color)

    # Save the image to a file
    print(image)
    image.save("crna.png")

if __name__ == "__main__":
    # Specify the resolution and color (RGB)
    width, height = 1200, 1200
    color = (0, 0, 0)

    # Create and save the image
    create_colored_image(width, height, color)
