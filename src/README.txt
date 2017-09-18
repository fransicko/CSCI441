Author: Michael Villafuerte
Assignment: 2

The program will display to the user a orcs sprite that is usspended in space. When the user presses a arrow key the sprite 
will move in that direction. When the sprite reaches the edge it will appear on the oppisite side of the screen still 
moving in which ever direction the user is pressing the arrow key. By clicking on the screen the user will change teh color
of the orc based on the posistion of the mouse.

User Section:
Run the program. Press or hold the arrow keys to move the sprite in the direction of the path.

Instructions:
	1. Open PowerShell and go to the directory that the main is located in.
	2. Type make.
	3. Run the a2.exe
	4. Press ESC to close the porgram when it is running.

Details:
The requirement for the mouse interations is simply click on a loactaion to change the color of the oorc and when
you move the mouse around his eye will follow it.


Time:
~ 4 hours

The lab helped me a lot since a lot of the code for this assignment was from the lab.

Fun:
9


Questions:

A. 
	// Assuming we have created our a time counter called "time", and we are incrementing it by one 
        // in a function to be placed some where in the code by the creater of the code.
 
	glm::mat4 transMtx = glm::translate(glm::mat4(), glm::vec3(x*cos(Theta.r + time), y*sin(Theta.r + time), 0));

		glMultMatrixf(&transMtx[0][0]); {

			drawHelmet();

		}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );


B. 
	// Assuming we have created our a time counter called "time", and we are incrementing it by one 
        // in a function to be placed some where in the code by the creater of the code.
 
	glm::mat4 transMtx = glm::translate(glm::mat4(), glm::vec3(i + x*cos(Theta.r + time), j + y*sin(Theta.r + time), 0));

		glMultMatrixf(&transMtx[0][0]); {

			drawHelmet();

		}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );


C.
	To shear the the matrix I would make a new matrix with that shear component then call the drawHelmet after it: 
	double shear[16] = {
   	 	1.0, shear, 0.0, 0.0,
    		1.0, 1.0, 0.0, 0.0,
    		0.0, 0.0, 1.0, 0.0,
    		0.0, 0.0, 0.0, 1.0
	};
	glMultMatrixf(shear);

	drawHelment();



