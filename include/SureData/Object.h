class SureObject
{
    public:
    SureObject();
    virtual ~SureObject();
    void MemFree();

    int external_id;
    int ParentID = -1;
    int CollideExcludeID = -1;
    my_double3 X;  //Координаты центра
    my_double3 ox; //Локальная ось x
    my_double3 oy; //Локальная ось y
    my_double3 oz; //Нормаль (Локальная ось z)
    my_double3 X_byparent;  //Координаты центра
    my_double3 ox_byparent; //Локальная ось x
    my_double3 oy_byparent; //Локальная ось y
    double lx; // длина
    double ly; // ширина
    double lz; // высота
    double lp = 4.0; // величина иннерциального тетраэдра
    double rig = 0.7; // жесткость
    double mass = 1.0;
    int type; // тип
    bool movable; // может двигаться
    bool collidable;
    my_double3 p1;
    my_double3 p2;
    my_double3 p3;
    my_double3 p4;
    my_double3 p1o;
    my_double3 p2o;
    my_double3 p3o;
    my_double3 p4o;
    int ModelID_collider;
    char ModelName_collider[20];
    int ModelID_drawable;
    char ModelName_drawable[20];
    SureDrawable drawable;

    SureParticle *Particles;
    int ParticlesCounter = 0;
    bool ParticlesInitiated = false;
    int ParticlesLimit = 0;
    double PSGravityMass = 1.0;
    bool PSInnerCollisions = false;
    bool PSSparks = false;

    int DrawableGPUID = -1;

    void align_byparent(SureObject *parent){
                    X = parent->X + X_byparent.x*parent->ox
                                  + X_byparent.y*parent->oy
                                  + X_byparent.z*parent->oz;
                    ox = __NORMALIZE( ox_byparent.x*parent->ox
                                    + ox_byparent.y*parent->oy
                                    + ox_byparent.z*parent->oz);
                    oy = __NORMALIZE( oy_byparent.x*parent->ox
                                    + oy_byparent.y*parent->oy
                                    + oy_byparent.z*parent->oz);
                    oz = __NORMALIZE(cross(ox,oy));
    }
    void initp4(){
                  // тэтраэдр создается чуть ниже чем нужно, поэтому при первом же выравнивании объект "проваливается" вниз
                  // чтобы этого не было -- костыль:
                  X=X+0.1*lp*oz;

                  p1 = X+ox*(SURE_P4_X*lp)-oz*(SURE_P4_Y*lp);
                  p2 = X-ox*(SURE_P4_Y*lp)+oy*(0.5*lp)-oz*(SURE_P4_Y*lp);
                  p3 = X-ox*(SURE_P4_Y*lp)-oy*(0.5*lp)-oz*(SURE_P4_Y*lp);
                  p4 = X+oz*(SURE_P4_Z*lp-SURE_P4_Y*lp);
                  p1o.x=p1.x;p1o.y=p1.y;p1o.z=p1.z;
                  p2o.x=p2.x;p2o.y=p2.y;p2o.z=p2.z;
                  p3o.x=p3.x;p3o.y=p3.y;p3o.z=p3.z;
                  p4o.x=p4.x;p4o.y=p4.y;p4o.z=p4.z;
    }
    void stop(){
        p1o = p1;
        p2o = p2;
        p3o = p3;
        p4o = p4;
    }
    void push(my_double3 pp,my_double3 pv,double pd)
    {
        if(type==SURE_OBJ_PS)return;

        my_double3 vp1 = pp-p1;
        double lp1 = __LENGTH(vp1);
        my_double3 vp2 = pp-p2;
        double lp2 = __LENGTH(vp2);
        my_double3 vp3 = pp-p3;
        double lp3 = __LENGTH(vp3);
        my_double3 vp4 = pp-p4;
        double lp4 = __LENGTH(vp4);

        my_double3 np5 = pp+pv*pd;

        double d;
        my_double3 vd;

        #define __ITER_5VD(A,B,C,D,L) \
        vd = A-B; \
        d = (L-__LENGTH(vd))*rig; \
        vd = __NORMALIZE(vd); \
        C += vd*d*0.5; \
        D -= vd*d*0.5;

        #define __ITER_5VDF(A,B,C,L) \
        vd = A-B; \
        d = (L-__LENGTH(vd))*rig; \
        vd = __NORMALIZE(vd); \
        C += vd*d;

        my_double3 np1=p1;
        my_double3 np2=p2;
        my_double3 np3=p3;
        my_double3 np4=p4;

        for(int i = 0;i<1;++i)
        {
            my_double3 d1 = my_double3{0,0,0};
            my_double3 d2 = my_double3{0,0,0};
            my_double3 d3 = my_double3{0,0,0};
            my_double3 d4 = my_double3{0,0,0};
            __ITER_5VD(np1,np2,d1,d2,lp);
            __ITER_5VD(np1,np3,d1,d3,lp);
            __ITER_5VD(np1,np4,d1,d4,lp);
            __ITER_5VDF(np1,np5,d1,lp1);
            __ITER_5VD(np2,np3,d2,d3,lp);
            __ITER_5VD(np2,np4,d2,d4,lp);
            __ITER_5VDF(np2,np5,d2,lp2);
            __ITER_5VD(np3,np4,d3,d4,lp);
            __ITER_5VDF(np3,np5,d3,lp3);
            __ITER_5VDF(np4,np5,d4,lp4);
            np1+=d1; np2+=d2; np3+=d3; np4+=d4; //np5+=d5;
        };

        p1=np1;
        p2=np2;
        p3=np3;
        p4=np4;
        align_p4();
        //movebyp4();
    }
    void next_tick(){
        my_double3 v1 = p1-p1o;
        my_double3 v2 = p2-p2o;
        my_double3 v3 = p3-p3o;
        my_double3 v4 = p4-p4o;
        // Затухание
        v1 = v1 - v1*0.001;
        v2 = v2 - v2*0.001;
        v3 = v3 - v3*0.001;
        v4 = v4 - v4*0.001;
        p1o=p1;p2o=p2;p3o=p3;p4o=p4;
        p1+=v1; p2+=v2; p3+=v3; p4+=v4;
        align_p4();
        movebyp4();
        if(type==SURE_OBJ_PS)
        for(int i=0;i<ParticlesCounter;++i){
            my_double3 NX = Particles[i].X + (Particles[i].X-Particles[i].OX);
            Particles[i].OX = Particles[i].X;
            Particles[i].X = NX;
        };
    }
    void align_p4(){
            double d;
            my_double3 pd;
            #define __ITER(A,B,C,D) \
            pd = A-B; \
            d = (lp-__LENGTH(pd))*rig; \
            pd = __NORMALIZE(pd); \
            C += pd*(d*0.5); \
            D -= pd*(d*0.5);
        //for(int i = 0;i<3;++i){
            my_double3 d1 = my_double3{0,0,0};
            my_double3 d2 = my_double3{0,0,0};
            my_double3 d3 = my_double3{0,0,0};
            my_double3 d4 = my_double3{0,0,0};
            __ITER(p1,p2,d1,d2);
            __ITER(p1,p3,d1,d3);
            __ITER(p1,p4,d1,d4);
            __ITER(p2,p3,d2,d3);
            __ITER(p2,p4,d2,d4);
            __ITER(p3,p4,d3,d4);
            p1+=d1;
            p2+=d2;
            p3+=d3;
            p4+=d4;
        //}; Iters
    }
    void teleport(my_double3 i_X){
        // перемещение без участя физики
        my_double3 MoveVec = i_X - X;
        p1+=MoveVec;
        p2+=MoveVec;
        p3+=MoveVec;
        p4+=MoveVec;
        p1o+=MoveVec;
        p2o+=MoveVec;
        p3o+=MoveVec;
        p4o+=MoveVec;
        movebyp4();
    }
    void movebyp4(){
        align_p4();
        oy=p2-p3;
        oz=cross(oy,p2-p1);
        ox=cross(oz,oy);
        ox=__NORMALIZE(ox);
        oy=__NORMALIZE(oy);
        oz=__NORMALIZE(oz);
        X=(p1+p2+p3+p4)*0.25;
    }
    my_double3 velbypp(my_double3 pp)
    {
        my_double3 vp1 = pp-p1o;
        double lp1 = __LENGTH(vp1);
        my_double3 vp2 = pp-p2o;
        double lp2 = __LENGTH(vp2);
        my_double3 vp3 = pp-p3o;
        double lp3 = __LENGTH(vp3);
        my_double3 vp4 = pp-p4o;
        double lp4 = __LENGTH(vp4);

        double d;
        my_double3 vd;

        #define __ITER_5VDFV(A,B,D,L) \
        vd = A-B; \
        d = (L-__LENGTH(vd))*rig; \
        vd = __NORMALIZE(vd); \
        D -= vd*d;

        my_double3 np1=p1;
        my_double3 np2=p2;
        my_double3 np3=p3;
        my_double3 np4=p4;
        my_double3 np5=pp;

        for(int i = 0;i<1;++i)
        {
            my_double3 d5 = my_double3{0,0,0};
            __ITER_5VDFV(np1,np5,d5,lp1);
            __ITER_5VDFV(np2,np5,d5,lp2);
            __ITER_5VDFV(np3,np5,d5,lp3);
            __ITER_5VDFV(np4,np5,d5,lp4);
            np5+=d5;
        };

        return np5-pp;
    };
};
