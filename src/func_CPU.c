
cl_double3 normalize(cl_double3 Vec)
{
    cl_double3 vr;
    double l = sqrt(Vec.s[0]*Vec.s[0] + Vec.s[1]*Vec.s[1] + Vec.s[2]*Vec.s[2]);
    vr.s[0] = Vec.s[0]/l; vr.s[1] = Vec.s[1]/l; vr.s[2] = Vec.s[2]/l;
    return vr;
};

my_double3 normalize(my_double3 Vec)
{
    my_double3 vr;
    double l = sqrt(Vec.x*Vec.x + Vec.y*Vec.y + Vec.z*Vec.z);
    vr.x = Vec.x/l; vr.y = Vec.y/l; vr.z = Vec.z/l;
    return vr;
};

cl_double3 cross(cl_double3 v1, cl_double3 v2)
{
    cl_double3 vr;
    vr.s[0] = v1.s[1]*v2.s[2] - v1.s[2]*v2.s[1];
    vr.s[1] = v1.s[2]*v2.s[0] - v1.s[0]*v2.s[2];
	vr.s[2] = v1.s[0]*v2.s[1] - v1.s[1]*v2.s[0];
	return vr;
};

cl_double dot(cl_double3 v1, cl_double3 v2)
{
    return v1.s[0]*v2.s[0] + v1.s[1]*v2.s[1] + v1.s[2]*v2.s[2];
};

my_double3 cross(my_double3 v1, my_double3 v2)
{
    my_double3 vr;
    vr.x = v1.y*v2.z - v1.z*v2.y;
    vr.y = - v1.x*v2.z + v1.z*v2.x;
	vr.z = v1.x*v2.y - v1.y*v2.x;
	return vr;
};

double dot(my_double3 v1, my_double3 v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
};

uint mad24(uint x,uint y,uint z)
{
    return x*y+z;
};

const my_double3 operator*(my_double3 a, double b)
{ return my_double3(a.x*b,a.y*b,a.z*b); };

const my_double3 operator*(double b, my_double3 a)
{ return my_double3(a.x*b,a.y*b,a.z*b); };

const my_double3 operator*(double b, my_uchar3 a)
{ return my_double3(a.x*b,a.y*b,a.z*b); };

const my_double3 operator-(my_double3 a, cl_float3 b)
{ return my_double3(a.x-b.s[0],a.y-b.s[1],a.z-b.s[2]); };

my_double3& operator+=(my_double3 &a,const my_double3 &b)
{ a.x+=b.x;a.y+=b.y;a.z+=b.z; return a; }

my_double3& operator-=(my_double3 &a,const my_double3 &b)
{ a.x-=b.x;a.y-=b.y;a.z-=b.z; return a; }



const cl_float3 operator-(cl_float3 a){
return cl_float3{-a.s[0],-a.s[1],-a.s[2]};
};

//const cl_float3 operator-(cl_float3 a,my_double3 b){
//return cl_float3{a.s[0]-b.x,a.s[1]-b.y,a.s[2]-b.z};
//};

void ObjCollide(SureObject* o1,SureObject* o2,my_double3 pp,my_double3 pd,double pl)
{
    if(pl<SURE_R_DELTA)return;
    my_double3 v1 = my_double3{0,0,0};
    my_double3 v2 = my_double3{0,0,0};

    if(o2->movable)o2->push(pp,pd,-pl*0.5);
    if(!o1->movable)o2->push(pp,pd,-pl*0.5);
    if(o1->movable)o1->push(pp,pd,pl*0.5);
    if(!o2->movable)o1->push(pp,pd,pl*0.5);

    if(o1->movable)v1 = o1->velbypp(pp);
    if(o2->movable)v2 = o2->velbypp(pp);
    my_double3 oz = __NORMALIZE(pd);
    double l_v1z = dot(oz,v1);
    double l_v2z = dot(oz,v2);
    my_double3 v1z = oz*l_v1z;
    my_double3 v1xy = v1-v1z;
    my_double3 v2z = oz*l_v2z;
    my_double3 v2xy = v2-v2z;

    if(o2->movable)o2->push(pp,v2xy,-1.0); // трение
    if(o1->movable)o1->push(pp,v1xy,-1.0); // трение

 }
