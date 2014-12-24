Image Compression
=================

This project aims to implement a basic image compression algorithm of mine.

Premise (Algorithm)
-------------------
* Consider each colour value of a pixel as representing a value of a dimension.
Thus, RGB values form co-ordinates in 3D space.
* The compressed form of an image consists of vertices of squares sampled from
the original image.
* An approximation of the original image can be reconstructed by interpolating
points on the plane formed by sets of three vertices.

Performance
-----------
Setting a square sample side of 4 yields a compressed file about half the size
of a JPEG of the same image.
Quality of the retreived image is yet to be examined.
&nbsp
Time complexity: theta(mn)
m_x_n : dimensions of the image.
