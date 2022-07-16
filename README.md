# Steganography

This application hides an image with in the a cover image and secured with encryption. You can hide and extract the image using your password. This application supports BMP, PNG, and JPEG files.

# Build Application

1. Run the Makefile with `make`

# Run Application

## Stego Image

1. Have your cover and secret image ready. Make sure that the cover image is 8x larger than the secret image. Then run the following command in your terminal.
   `./dcstego -t stego -cm <cover image path> -sm <secret image path>`

2. Then enter a password for the encryption. You will use this password to decrypt the image later.

3. Then enter a name for the new image.

4. The new image will be created in the same directory and will look exectly like the cover image.

## Un-Stego Image

1. To unstego the new image you created, run the following command:
   `./dcstego -t unstego -cm <name of stegoed image>`

2. Then enter the password you used to stego the image.

3. Then enter a name for the new extracted image.

4. The new image will be created in the same directory and will look exectly like the cover image.

# Documentation

View the full documentation with design, how-to guide, and test cases in `./Documents`. You can also view the application in action in `./Videos`.
