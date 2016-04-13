/* *
 *
 * Author: Pujan Dave
 *
 * */


# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <math.h>
# include "opencv2/highgui/highgui.hpp"
# include "opencv2/imgproc/imgproc.hpp"


using namespace cv;
using namespace std;

# define OFFSET_PICT 0.1
# define DST_SIZE 200


Mat scaleRotateTranslate(Mat img, double angle, int center_x, int center_y){

   Point2f srcTri[3];
   Point2f dstTri[3];
   
   int a, b, c, d, e,f;

   Mat rot_mat( 2, 3, CV_32FC1 );
   Mat warp_mat( 2, 3, CV_32FC1 );
   Mat src, warp_dst, warp_rotate_dst;

   /// Load the image
   src = img;

   /// Set the dst image the same type and size as src
   warp_dst = Mat::zeros( src.rows, src.cols, src.type() );


   a = cos(angle);
   b = sin(angle);
   c = center_x - center_x * a - center_y * b;
   d = -b;
   e = a;
   f = center_y - center_x * d - center_y * e;

   srcTri[0] = Point2f( 0,0 );
   srcTri[1] = Point2f( src.cols - 1, 0 );
   srcTri[2] = Point2f( 0, src.rows - 1 );

   dstTri[0] = Point2f( c, f );
   dstTri[1] = Point2f( (src.cols-1)*a + c, d*(src.cols-1) + f );
   dstTri[2] = Point2f( (src.cols-1)*b + c, (src.cols-1)*e + f );

   /// Get the Affine Transform
   warp_mat = getAffineTransform( srcTri, dstTri );

   /// Apply the Affine Transform just found to the src image
   warpAffine( src, warp_dst, warp_mat, warp_dst.size() );

   /** Rotating the image after Warp */

   /// Compute a rotation matrix with respect to the center of the image
   //Point center = Point( center_x, center_y );
   //double scale = 1;

   /// Get the rotation matrix with the specifications above
   //rot_mat = getRotationMatrix2D( center, angle, scale );

   /// Rotate the warped image
   //warpAffine( warp_dst, warp_rotate_dst, rot_mat, warp_dst.size() );

   return warp_dst;
}


int crop(string src, int eye_lx, int eye_ly, int eye_rx, int eye_ry, sting dst){

	int offset_h, offset_v;
	int eye_directionX, eye_directionY;
	double rotation, dist, reference, scale, crop_x, crop_y, crop_w, crop_h;

	Mat img = imread(src, CV_LOAD_IMAGE_UNCHANGED);	// Read the image from the src file

	// Error Check!
	if(img.empty()){
		cout<<"Couldn't load the image: "<<src<<" !!!"<<endl;
		return -1;
	}

	// Offsets for original image
	offset_h = floor((double)OFFSET_PICT*DST_SIZE);
	offset_v = floor((double)OFFSET_PICT*DST_SIZE);

	// Get the direction
	eye_directionX = eye_rx - eye_lx;
	eye_directionY = eye_ry - eye_ly;

	// Rotation angles in radians
	rotation = - (atan2((double)eye_directionY,(double)eye_directionX));

	// Distance between 2 eyes
	dist = sqrt((eye_rx - eye_lx)*(eye_rx - eye_lx) + (eye_ry - eye_ly)*(eye_ry - eye_ly));
	// reference eye-width
	reference = DST_SIZE - (2.0 * offset_h);
	// Scale factor
	scale = dist/reference;

	// Rotate the original image around the left eye
	img = scaleRotateTranslate(img, rotation, eye_lx, eye_ly);

	// crop the rotated image!
	crop_x = eye_lx - scale*offset_h;
	crop_y = eye_ly - scale*offset_v;
	crop_w = DST_SIZE*scale;
	crop_h = DST_SIZE*scale;

	imwrite(dst, img(Rect(crop_x, crop_y, crop_w, crop_h)));

	return 0;
}











