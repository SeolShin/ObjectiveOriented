#include "CaptureTag.h"


int CaptureTag::StartCapture()
{
	//glutInit(&argc, argv);
    //if( init() < 0 ) exit(0);

    arVideoCapStart();
    //glutMotionFunc( this->motionEvent );
	//argMainLoop( NULL, NULL, this->mainLoop ); //thread

	return (0);
}

void CaptureTag::mainLoop(void)
{
    ARUint8         *dataPtr;
    ARMarkerInfo    *marker_info;
    int             marker_num;
    int             j, k;
    
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return;
	}

    //glClearColor( 0.0, 0.0, 0.0, 0.0 );
    //glClearDepth( 1.0 );
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    argDrawMode2D();
    if( disp_mode ) {
        argDispImage( dataPtr, 0, 0 );
	}
    else {
        argDispImage( dataPtr, 1, 1 );
	}
    
    if( arDetectMarker(dataPtr, thresh, &marker_info, &marker_num) < 0 ) {
        cleanup();
        exit(0);
	}
    arVideoCapNext();

    if( arDebug ) {
        if( arImageProcMode == AR_IMAGE_PROC_IN_HALF )
            argDispHalfImage( arImage, 2, 1 );
        else
            argDispImage( arImage, 2, 1);
    }

    
    k = -1;
    for( j = 0; j < marker_num; j++ ) {
        if( marker_info[j].id == target_id ) {
            if( k == -1 ) k = j;
            else {
                if( marker_info[k].cf < marker_info[j].cf ) k = j;
            }
        }
    }
    if( k != -1 ) {
       // glDisable(GL_DEPTH_TEST);
        switch( outputMode ) {
            case 0:
                getResultRaw( &marker_info[k] );
                break;
            case 1:
                getResultQuat( &marker_info[k] );
                break;
        }
    }

    argSwapBuffers();
}


 void CaptureTag::getResultRaw( ARMarkerInfo *marker_info )
 {
    double      target_trans[3][4];
    double      cam_trans[3][4];
    char        string[256];

    if( arGetTransMat(marker_info, target_center, target_width, target_trans) < 0 ) return;
    if( arUtilMatInv(target_trans, cam_trans) < 0 ) return;

    sprintf(string," RAW: Cam Pos x: %3.1f  y: %3.1f  z: %3.1f",
            cam_trans[0][3], cam_trans[1][3], cam_trans[2][3]);

	/*
	if( disp_mode ) {
        draw( "target", target_trans, 0, 0 );
        draw_exview( a, b, r, target_trans, 1, 1 );
    }
    else {
        draw( "target", target_trans, 1, 1 );
        draw_exview( a, b, r, target_trans, 0, 0 );
    }
	*/
    //print_string( string );

    return;
}

 void CaptureTag::getResultQuat( ARMarkerInfo *marker_info )
{
    double      target_trans[3][4];
    double      cam_trans[3][4];
    double      quat[4], pos[3];
    char        string1[256];
    char        string2[256];

    if( arGetTransMat(marker_info, target_center, target_width, target_trans) < 0 ) return;
    if( arUtilMatInv(target_trans, cam_trans) < 0 ) return;
    if( arUtilMat2QuatPos(cam_trans, quat, pos) < 0 ) return;

    sprintf(string1," QUAT: Pos x: %3.1f  y: %3.1f  z: %3.1f\n", //Pos x, Pos y, Pos z
            pos[0], pos[1], pos[2]);
    sprintf(string2, "      Quat qx: %3.2f qy: %3.2f qz: %3.2f qw: %3.2f ",
            quat[0], quat[1], quat[2], quat[3]);
    strcat( string1, string2 );
/*
    if( disp_mode ) {
        draw( "target", target_trans, 0, 0 );
        draw_exview( a, b, r, target_trans, 1, 1 );
    }
    else {
        draw( "target", target_trans, 1, 1 );
        draw_exview( a, b, r, target_trans, 0, 0 );
    }
*/
    //print_string( string1 );
    return;
}

/* set up the application parameters - read in from command line*/
 int CaptureTag::init(void)
{
    char     cparaname[256];
    char     pattname[256];
    ARParam  wparam;

    strcpy( cparaname, "Data/camera_para.dat" );
    strcpy( pattname,  "Data/patt.hiro" );
    
    /* open the video path */
    if( arVideoOpen( vconf ) < 0 ) exit(0);
    /* find the size of the window */
    if( arVideoInqSize(&xsize, &ysize) < 0 ) exit(0);
    printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

    /* set the initial camera parameters */
    if( arParamLoad(cparaname, 1, &wparam) < 0 ) {
       printf("Camera parameter load error !!\n");
        exit(0);
    }
    arParamChangeSize( &wparam, xsize, ysize, &cparam );
    arInitCparam( &cparam );
    printf("*** Camera Parameter ***\n");
    arParamDisp( &cparam );

    /* open the graphics window */
    argInit( &cparam, 1.0, 0, 2, 1, 0 );

    if( (target_id = arLoadPatt(pattname)) < 0 ) {
        printf("Target pattern load error!!\n");
        exit(0);
    }

    arDebug = 0;

    return 0;
}

/* cleanup function called when program exits */
 void CaptureTag::cleanup(void)
{
    arVideoCapStop();
    arVideoClose();
    argCleanup();
}