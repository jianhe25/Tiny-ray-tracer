The first assignment in CS184, it's relatively easy.

The tasks:
* Implement a crystal ball scene to observe a teapot, the crystal ball means one viewer are moving around the surface of sphere, you can control viewer by left/right, and up/down keys.
* Implement glulookAt(eye, center, up)

For task 1, I can use sphere coordinate to represent the viewer, and when move the key, change the longtitude and latitude, second choice is do move it directly in Descartes coordiante,
and the movement can be represented by rotation around the x_axis or y_axis. One important thing to notice is how you move
the up vector. (the up vector is actually the norm vector at sphere surface, because there are infinite norm vector at sphere surface
you have to calculate it in each iteration, and I didn't get the right solution now. I calculate it by viewer's position cross product x_axis, which is wrong.


## Orignal teapot
![original teapot](https://raw.github.com/jianhe25/ComputerGraphics-Berkeley-CS184/master/hw1-windows/input.txt.000.png)

## Left Rotate
![original teapot](https://raw.github.com/jianhe25/ComputerGraphics-Berkeley-CS184/master/hw1-windows/input.txt.008.png)
