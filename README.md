Function: generate random circles on an image, returning results in a .txt file with 0 and 1 as no-hits and hits. 
Details: n circles are generated with Bresenham Circle Algorithm, user can define a interval [a, b] from which n is randomly chosen using mersenne twister
  background noise is randomly generated
  user set image size, list of values for circle radii, noise rate, minimum and maximum number [a, b] of circles

2 main versions to use: _v04 and _v05 depending on requirements

_v04 generate single .txt file with images with following format:
  n r1 x1 y1 ... rn xn yn image
  with
  n = total number of circles
  ri, xi, yi = radius and coordinate of circle center with i elem of [1, n]
  image = a string of 0 and 1 depending on pixels being hits or not.

_v05 generate two .txt files, ending in _p.txt and _cc.txt
  _p.txt contains lists of radii and circle centers coordinates ri, xi, yi, each list for different images
  _cc.txt contains list of all positive hits in image, ignores all empty pixels
  mainly intended because of smaller sizes

TLDR.: _v04 generate images, _v05 generate list of coordinates
