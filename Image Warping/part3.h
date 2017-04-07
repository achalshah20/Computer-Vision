double bilinear_int(CImg<double> &in,double x,double y,int c)
{

  int i,j;
  double a,b,first_pt,second_pt,third_pt,forth_pt;

  if(x < 0 or y < 0 )
  {
    // cout << "inside bracket" << endl;
    return 0.0;
  }  

  else
  {
    // cout << "insice else" << endl;
    i =floor(x);
    j =floor(y);
    
    // cout << "test3" << endl;
    a = x - i;
    b = y - j;

    // cout << "test4" << endl;

    // cout << x << " " << y << " " << " " << c << " " << endl;
     
    first_pt = (1-a)*(1-b)*in(i,j,c);
    second_pt = a*(1-b)*in(i+1,j,c);
    third_pt = a*b*in(i+1,j+1,c);
    forth_pt = (1-a)*b*in(i,j+1,c);
    
    // cout << "test5" << endl;
// 
    // cout << first_pt << " " <<sec_pt<< " " << third_pt <<" " <<frth_pt << " " << endl;

    return first_pt+second_pt+third_pt+forth_pt; 

  }


}

void warp_image(CImg<double> &in,const CImg<double> &matrix,CImg<double> &out)
{

  double  z = 1;
  double _x,_y,_z;
  double val;
  
  cimg_forXYC(in,x,y,v)
    { 
      
      _x = matrix(0,0)*x + matrix(0,1)*y + matrix(0,2)*z;
      _y = matrix(1,0)*x + matrix(1,1)*y + matrix(1,2)*z;
      _z = matrix(2,0)*x + matrix(2,1)*y + matrix(2,2)*z;

      _x = (_x/_z);
      _y = (_y/_z);
      
      if( _x < 0)
        {_x = 0;}
      else if (_x > in.width())
        { _x = 0.0;}
      
      // if (_x > in.width() or _y > in.height())
      // {
      //   out(x,y,v) = 0.0;
      // }
      
              

      if ( _y < 0)
         {_y = 0.0;}
      else if (_y > in.height())
         {_y = 0;}
        
        // cout << " bilinear int called" << endl;
        // cout << _x << " " << _y << " "<< " " << v<< " " << endl;
       val = bilinear_int(in,_x,_y,v);
       // cout << " bilinear completed" << endl;

       
       out(x,y,v) = val;

    };

    
   
}






