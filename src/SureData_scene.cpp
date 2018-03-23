

void SureData::Scene_box() // коробка со светящимся потолком
{
__VTYPE3 X;
  // свет
    TemplateObject.type = SURE_OBJ_PLANE;
    TemplateObject.drawable.advmap_id = -1;
    TemplateObject.drawable.map_id = -1;
    X.x = 0; X.y = 0; X.z = 100;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = -1;
    TemplateObject.lx = 50.0;
    TemplateObject.ly = 50.0;
    TemplateObject.lz = 1.0;
    TemplateObject.movable = false;
    TemplateObject.collidable = true;
    TemplateObject.drawable.type = SURE_DR_SQUARE; // форма
    TemplateObject.drawable.radiance = 1.0; // свечение
    TemplateObject.drawable.rgb.s[0] = 250; // цвет
    TemplateObject.drawable.rgb.s[1] = 250;
    TemplateObject.drawable.rgb.s[2] = 200;

    CreateObjectFromTemplate(&X);

    X.x = 0; X.y = 0; X.z = 0;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.drawable.map_id = GetTexture("parket");
    TemplateObject.drawable.radiance = 0.0; // свечение
    TemplateObject.drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    TemplateObject.drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    TemplateObject.drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет

    CreateObjectFromTemplate(&X); // пол

    TemplateObject.drawable.refr = 1.01; // Коэффициент преломления
    TemplateObject.drawable.dist_type = SURE_D_NORM; // тип рандомизации
    TemplateObject.drawable.dist_sigma = 0.01; // sigma рандомизации
    X.x = 50; X.y = 0; X.z = 50;
    TemplateObject.ox.x = 0; TemplateObject.ox.y = 0; TemplateObject.ox.z = 1;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = -1; TemplateObject.oz.y = 0; TemplateObject.oz.z = 0;
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    TemplateObject.drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    TemplateObject.drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет

    CreateObjectFromTemplate(&X); // cтена +x

    X.x = -50; X.y = 0; X.z = 50;
    TemplateObject.ox.x = 0; TemplateObject.ox.y = 0; TemplateObject.ox.z = 1;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 1; TemplateObject.oz.y = 0; TemplateObject.oz.z = 0;
    TemplateObject.drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    TemplateObject.drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    TemplateObject.drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет

    CreateObjectFromTemplate(&X);    // стена -x

    X.x = 0; X.y = 50; X.z = 50;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 0; TemplateObject.oy.z = 1;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = -1; TemplateObject.oz.z = 0;
    TemplateObject.drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    TemplateObject.drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    TemplateObject.drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет

    CreateObjectFromTemplate(&X);  // стена +y

    X.x = 0; X.y = -50; X.z = 50;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 0; TemplateObject.oy.z = 1;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 1; TemplateObject.oz.z = 0;
    TemplateObject.drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    TemplateObject.drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    TemplateObject.drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет

    CreateObjectFromTemplate(&X);  // стена -y

   TemplateObject.type = SURE_OBJ_PLANE;
    TemplateObject.lx = 600.0;
    TemplateObject.ly = 600.0;
    TemplateObject.lz = 1.0;
    TemplateObject.drawable.type = SURE_DR_SQUARE; // форма
    X.x = 0; X.y = 0; X.z = 25;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.drawable.map_id = -1;//
    TemplateObject.drawable.radiance = 0.0; // свечение
    TemplateObject.drawable.dist_type = SURE_D_EQUAL;
    TemplateObject.drawable.transp = 1.1f;
    TemplateObject.drawable.transp_i = 0.005f;
    TemplateObject.drawable.sided = false;
    TemplateObject.collidable = false;
    TemplateObject.drawable.rgb.s[0] = 170; // цвет
    TemplateObject.drawable.rgb.s[1] = 150; // цвет
    TemplateObject.drawable.rgb.s[2] = 150; // цвет

    CreateObjectFromTemplate(&X); // пол туман
};


void SureData::Scene_floor() // Пол и круглая лампа
{
__VTYPE3 X;
    // пол
    TemplateObject.type = SURE_OBJ_PLANE;
    TemplateObject.lx = 60.0;
    TemplateObject.ly = 60.0;
    TemplateObject.lz = 1.0;
    TemplateObject.drawable.type = SURE_DR_SQUARE; // форма
    X.x = 0; X.y = 0; X.z = 0.0f;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.drawable.map_id = GenTexture("scells",SURE_GENTEX_UNTRANSP);
    TemplateObject.drawable.advmap_id = -1;
    TemplateObject.drawable.radiance = 0.0; // свечение
    TemplateObject.drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    TemplateObject.drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    TemplateObject.drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет

    CreateObjectFromTemplate(&X); // пол

    TemplateObject.type = SURE_OBJ_SPHERE;
    TemplateObject.lx = 70.0;
    TemplateObject.drawable.type = SURE_DR_SPHERE; // форма
    X.x = 90; X.y = 90; X.z = 150;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = -1;
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
    TemplateObject.drawable.radiance = 1.0; // свечение
    TemplateObject.drawable.rgb.s[0] = 250; // цвет
    TemplateObject.drawable.rgb.s[1] = 250; // цвет
    TemplateObject.drawable.rgb.s[2] = 230; // цвет

    CreateObjectFromTemplate(&X);   // свет

    TemplateObject.type = SURE_OBJ_PLANE;
    TemplateObject.lx = 600.0;
    TemplateObject.ly = 600.0;
    TemplateObject.lz = 1.0;
    TemplateObject.drawable.type = SURE_DR_SQUARE; // форма
    X.x = 0; X.y = 0; X.z = 20;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.drawable.map_id = -1;//
    TemplateObject.drawable.radiance = 0.0; // свечение
    TemplateObject.drawable.dist_type = SURE_D_EQUAL;
    TemplateObject.drawable.transp = 1.1f;
    TemplateObject.drawable.transp_i = 0.005f;
    TemplateObject.drawable.sided = false;
    TemplateObject.drawable.rgb.s[0] = 170; // цвет
    TemplateObject.drawable.rgb.s[1] = 150; // цвет
    TemplateObject.drawable.rgb.s[2] = 150; // цвет

    CreateObjectFromTemplate(&X); // пол туман

};

void SureData::Scene_mirrors() // Пол и зеркала
{
__VTYPE3 X;

    TemplateObject.type = SURE_OBJ_SPHERE;
    TemplateObject.lx = 20.0;
    TemplateObject.drawable.type = SURE_DR_SPHERE; // форма
    X.x = 0; X.y = 0; X.z = 0;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = -1;
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
    TemplateObject.drawable.radiance = 1.0; // свечение
    TemplateObject.drawable.rgb.s[0] = 250; // цвет
    TemplateObject.drawable.rgb.s[1] = 250; // цвет
    TemplateObject.drawable.rgb.s[2] = 230; // цвет

    CreateObjectFromTemplate(&X);   // свет

    // пол
    TemplateObject.type = SURE_OBJ_PLANE;
    TemplateObject.lx = 30.0;
    TemplateObject.ly = 30.0;
    TemplateObject.lz = 1.0;
    TemplateObject.drawable.type = SURE_DR_SQUARE; // форма
    X.x = 0; X.y = 0; X.z = -60;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.drawable.map_id = GetTexture("colstones");
    TemplateObject.drawable.advmap_id = GetTexture("test_adv");
    TemplateObject.drawable.radiance = 0.0; // свечение
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.drawable.dist_sigma = 0.3;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.rgb.s[0] = 255.0; // цвет
    TemplateObject.drawable.rgb.s[1] = 255.0; // цвет
    TemplateObject.drawable.rgb.s[2] = 255.0; // цвет

    //CreateObjectFromTemplate(&X); // пол

    X.x = 0; X.y = 0; X.z = 60;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = -1;
    //CreateObjectFromTemplate(&X);

    for(float VerticalAngle = 0.5;VerticalAngle<M_PI;VerticalAngle+=0.5)
    for(float HorisontalAngle = 0;HorisontalAngle<(2*M_PI);HorisontalAngle+=0.5){
        X.x = 160.0 * sin(HorisontalAngle) * sin(VerticalAngle); X.y = 160.0 * cos(HorisontalAngle) * sin(VerticalAngle); X.z = 160.0 * cos(VerticalAngle);// * cos(VerticalAngle);
        __VTYPE3 vec = -X;
        TemplateObject.oz = __NORMALIZE(vec);
        __VTYPE3 Oy = {-TemplateObject.oz.z,0.0f,-TemplateObject.oz.x};
        TemplateObject.ox = __NORMALIZE(Oy);
        TemplateObject.oy = cross(TemplateObject.oz,TemplateObject.ox);
        CreateObjectFromTemplate(&X);
    };

};

void SureData::Scene_ManySpheres()
{
__VTYPE3 X;
    TemplateObject.type = SURE_OBJ_SPHERE;
    TemplateObject.lx = 20.0;
    TemplateObject.drawable.type = SURE_DR_SPHERE; // форма
    X.x = 0; X.y = 0; X.z = 0;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = -1;
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
    TemplateObject.drawable.radiance = 1.0; // свечение
    TemplateObject.drawable.rgb.s[0] = 250; // цвет
    TemplateObject.drawable.rgb.s[1] = 250; // цвет
    TemplateObject.drawable.rgb.s[2] = 230; // цвет

    CreateObjectFromTemplate(&X);   // свет

TemplateObject.type = SURE_OBJ_SPHERE;
    TemplateObject.lx = 30.0;
    TemplateObject.ly = 50.0;
    TemplateObject.lz = 10.0;
    TemplateObject.drawable.type = SURE_DR_SPHERE; // форма
    //sprintf(TemplateObject.ModelName_collider,"tetr");
    TemplateObject.ModelID_collider = -1;//GetModel(TemplateObject.ModelName_collider);
    //sprintf(TemplateObject.ModelName_drawable,"tetr");
    TemplateObject.ModelID_drawable = -1;//GetModel(TemplateObject.ModelName_drawable);
    X.x = 0; X.y = 0; X.z = -60;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.drawable.map_id = -1; //GetTexture("colstones");
    TemplateObject.drawable.advmap_id = -1; //GetTexture("test_adv");
    TemplateObject.drawable.radiance = 0.0; // свечение
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.drawable.dist_m = 0.0;
    TemplateObject.drawable.rgb.s[0] = 255.0*((float)rand()/(float)RAND_MAX); // цвет
    TemplateObject.drawable.rgb.s[1] = 255.0*((float)rand()/(float)RAND_MAX); // цвет
    TemplateObject.drawable.rgb.s[2] = 255.0*((float)rand()/(float)RAND_MAX); // цвет

    X.x = 0; X.y = 0; X.z = 60;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = -1;
    //CreateObjectFromTemplate(&X);

    for(float VerticalAngle = 0.2;VerticalAngle<M_PI;VerticalAngle+=0.2)
    for(float HorisontalAngle = 0;HorisontalAngle<(2*M_PI);HorisontalAngle+=0.3){
        X.x = 130.0 * sin(HorisontalAngle) * sin(VerticalAngle); X.y = 130.0 * cos(HorisontalAngle) * sin(VerticalAngle); X.z = 130.0 * cos(VerticalAngle);// * cos(VerticalAngle);
        __VTYPE3 vec = -X;
        TemplateObject.oz = __NORMALIZE(vec);
        __VTYPE3 Oy = {-TemplateObject.oz.z,0.0f,-TemplateObject.oz.x};
        TemplateObject.ox = __NORMALIZE(Oy);
        TemplateObject.oy = cross(TemplateObject.oz,TemplateObject.ox);
        TemplateObject.lx = (20.0*(float)rand()/(float)RAND_MAX+3.0)*(sin(VerticalAngle)+0.3);
        TemplateObject.drawable.rgb.s[0] = 255.0*((float)rand()/(float)RAND_MAX); // цвет
        TemplateObject.drawable.rgb.s[1] = 255.0*((float)rand()/(float)RAND_MAX); // цвет
        TemplateObject.drawable.rgb.s[2] = 255.0*((float)rand()/(float)RAND_MAX); // цвет
        TemplateObject.drawable.dist_sigma = (float)rand()/(float)RAND_MAX;
        CreateObjectFromTemplate(&X);
    };


};

void SureData::Scene_ManyTetrs()
{
__VTYPE3 X;

    TemplateObject.type = SURE_OBJ_SPHERE;
    TemplateObject.lx = 20.0;
    TemplateObject.drawable.type = SURE_DR_SPHERE; // форма
    X.x = 0; X.y = 0; X.z = 0;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = -1;
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
    TemplateObject.drawable.radiance = 1.0; // свечение
    TemplateObject.drawable.rgb.s[0] = 250; // цвет
    TemplateObject.drawable.rgb.s[1] = 250; // цвет
    TemplateObject.drawable.rgb.s[2] = 230; // цвет

    CreateObjectFromTemplate(&X);   // свет

    // пол
    TemplateObject.type = SURE_OBJ_MESH;
    TemplateObject.lx = 50.0;
    TemplateObject.ly = 50.0;
    TemplateObject.lz = 10.0;
    TemplateObject.drawable.type = SURE_DR_MESH; // форма
    sprintf(TemplateObject.ModelName_collider,"tetr");
    TemplateObject.ModelID_collider = GetModel(TemplateObject.ModelName_collider);
    sprintf(TemplateObject.ModelName_drawable,"tetr");
    TemplateObject.ModelID_drawable = GetModel(TemplateObject.ModelName_drawable);
    X.x = 0; X.y = 0; X.z = -60;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.drawable.map_id = GetTexture("colstones");
    TemplateObject.drawable.advmap_id = GetTexture("test_adv");
    TemplateObject.drawable.radiance = 0.0; // свечение
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.drawable.dist_sigma = 1.5;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.rgb.s[0] = 255.0; // цвет
    TemplateObject.drawable.rgb.s[1] = 255.0; // цвет
    TemplateObject.drawable.rgb.s[2] = 255.0; // цвет

    //CreateObjectFromTemplate(&X); // пол

    X.x = 0; X.y = 0; X.z = 60;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = -1;
    //CreateObjectFromTemplate(&X);

    for(float VerticalAngle = 0.2;VerticalAngle<M_PI;VerticalAngle+=0.2)
    for(float HorisontalAngle = 0;HorisontalAngle<(2*M_PI);HorisontalAngle+=0.3){
        X.x = 130.0 * sin(HorisontalAngle) * sin(VerticalAngle); X.y = 130.0 * cos(HorisontalAngle) * sin(VerticalAngle); X.z = 130.0 * cos(VerticalAngle);// * cos(VerticalAngle);
        __VTYPE3 vec = -X;
        TemplateObject.oz = __NORMALIZE(vec);
        __VTYPE3 Oy = {-TemplateObject.oz.z,0.0f,-TemplateObject.oz.x};
        TemplateObject.ox = __NORMALIZE(Oy);
        TemplateObject.oy = cross(TemplateObject.oz,TemplateObject.ox);
        CreateObjectFromTemplate(&X);
    };



};

void SureData::Scene_tetrs() // Пол и лампа
{

__VTYPE3 X;

    TemplateObject.type = SURE_OBJ_MESH;
    TemplateObject.drawable.advmap_id = -1;
    TemplateObject.drawable.map_id = GenTexture("scells",SURE_GENTEX_UNTRANSP);
    X.x = 0; X.y = 0; X.z = -155;
    TemplateObject.ox.x = 0; TemplateObject.ox.y = 1; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 1; TemplateObject.oy.y = 0; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = -1;

    //TemplateObject.oz.x = 0.1; TemplateObject.ox.y = -0.2; TemplateObject.ox.z = -0.9;
    //TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    //TemplateObject.oy = __NORMALIZE(PenVec(TemplateObject.oz));
    //TemplateObject.ox = cross(TemplateObject.oz,TemplateObject.ox);

    TemplateObject.lx = 600.0;
    TemplateObject.ly = 600.0;
    TemplateObject.lz = 100.0;
    TemplateObject.lp = 100.0;
    TemplateObject.movable = false;
    TemplateObject.collidable = true;
    TemplateObject.drawable.sided = false;
    TemplateObject.drawable.type = SURE_DR_MESH; // форма
    TemplateObject.drawable.radiance = 0.0; // свечение
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.dist_sigma = 3.0f;
    sprintf(TemplateObject.ModelName_collider,"tetr");
    TemplateObject.ModelID_collider = GetModel(TemplateObject.ModelName_collider);
    sprintf(TemplateObject.ModelName_drawable,"tetr");
    TemplateObject.ModelID_drawable = GetModel(TemplateObject.ModelName_drawable);

    TemplateObject.drawable.refr = 1.01; // Коэффициент преломления
    TemplateObject.drawable.dist_type = SURE_D_EQUAL; // тип рандомизации
    TemplateObject.drawable.dist_sigma = 1.0f; // sigma рандомизации


    CreateObjectFromTemplate(&X);   //пол


    TemplateObject.type = SURE_OBJ_MESH;
    TemplateObject.drawable.map_id = -1;
    X.x = 0; X.y = 0; X.z = 200;
    TemplateObject.ox.x = 0; TemplateObject.ox.y = 1; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 1; TemplateObject.oy.y = 0; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = -1;
    TemplateObject.lx = 160.0;
    TemplateObject.ly = 160.0;
    TemplateObject.lz = 30.0;
    TemplateObject.movable = false;
    TemplateObject.collidable = true;
    TemplateObject.drawable.type = SURE_DR_MESH; // форма
    TemplateObject.drawable.radiance = 1.0; // свечение
    TemplateObject.drawable.rgb.s[0] = 250; // цвет
    TemplateObject.drawable.rgb.s[1] = 250;
    TemplateObject.drawable.rgb.s[2] = 200;
  CreateObjectFromTemplate(&X); // свет

    TemplateObject.type = SURE_OBJ_SPHERE;
    TemplateObject.lx = 50.0;
    TemplateObject.ly = 50.0;
    TemplateObject.lz = 10.0;
    TemplateObject.drawable.type = SURE_DR_SPHERE; // форма
    //sprintf(TemplateObject.ModelName_collider,"tetr");
    TemplateObject.ModelID_collider = -1;//GetModel(TemplateObject.ModelName_collider);
    //sprintf(TemplateObject.ModelName_drawable,"tetr");
    TemplateObject.ModelID_drawable = -1;//GetModel(TemplateObject.ModelName_drawable);
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.drawable.map_id = -1; //GetTexture("colstones");
    TemplateObject.drawable.advmap_id = -1; //GetTexture("test_adv");
    TemplateObject.drawable.radiance = 0.0; // свечение
    TemplateObject.drawable.refr = 1.1f;
    TemplateObject.drawable.transp = 0.99f;
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.drawable.dist_sigma = 0.01f;
    TemplateObject.drawable.dist_m = 0.0;
    TemplateObject.drawable.rgb.s[0] = 150.0f; // цвет
    TemplateObject.drawable.rgb.s[1] = 150.0f; // цвет
    TemplateObject.drawable.rgb.s[2] = 240.0f; // цвет

    X.x = -100; X.y = 0; X.z = 0;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = -1;
    //CreateObjectFromTemplate(&X);

         CreateObjectFromTemplate(&X);


};


void SureData::Scene_golem()
{
__VTYPE3 X;

    X.x = 0;
    X.y = 0;
    X.z = 20;
    TemplateObject.type = SURE_OBJ_MESH;
    TemplateObject.lx = 20.0;
    TemplateObject.ly = 20.0;
    TemplateObject.lz = 20.0;
    TemplateObject.lp = 30;
    //TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    //TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    //TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.ox.x = 0; TemplateObject.ox.y = 1; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 0; TemplateObject.oy.z = 1;
    TemplateObject.oz.x = 1; TemplateObject.oz.y = 0; TemplateObject.oz.z = 0;
    sprintf(TemplateObject.ModelName_drawable,"golem");
    TemplateObject.ModelID_drawable = GetModel(TemplateObject.ModelName_drawable);
    sprintf(TemplateObject.ModelName_collider,"cube");
    TemplateObject.ModelID_collider = GetModel(TemplateObject.ModelName_collider);
    TemplateObject.drawable.map_id = GetTexture("golem");//GetTexture("ghost_dark");//GenTexture("scells",SURE_GENTEX_UNTRANSP);
    //MapTexture(TemplateObject.ModelID_drawable,SURE_MAPPING_SPHERICAL);
    TemplateObject.drawable.advmap_id = GetTexture("golem_adv");
    TemplateObject.drawable.dist_type = SURE_D_EQUAL;//SURE_D_NORM;
    TemplateObject.drawable.dist_sigma = 0.005f;
    TemplateObject.drawable.rgb.s[0] = 240;
    TemplateObject.drawable.rgb.s[1] = 240;
    TemplateObject.drawable.rgb.s[2] = 253;

    TemplateObject.drawable.type = SURE_DR_MESH;
    TemplateObject.drawable.sided = false;
    TemplateObject.drawable.radiance = 0;

    CreateObjectFromTemplate(&X);
};
/*
void SureData::Scene_metaball(double i_x,double i_y,double i_z,double i_sz,int nt) // Пол и круглая лампа
{

    int i;
    my_double3 generated_mesh[256];

    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = i_x; //Координаты центра
    objects[i].X.y = i_y;
    objects[i].X.z = i_z;
    objects[i].lx = i_sz; // длина
    objects[i].ly = i_sz; // ширина
    objects[i].lz = i_sz; // высота

    for(int mi = 0;mi<200;++mi)
    {
        my_double3 tm;
        tm.x = (float)rand()/(float)RAND_MAX - 0.5;
        tm.y = (float)rand()/(float)RAND_MAX - 0.5;
        tm.z = (float)rand()/(float)RAND_MAX - 0.5;
        generated_mesh[mi] = __NORMALIZE(tm);
        generated_mesh[mi].x *= objects[i].lx;
        generated_mesh[mi].y *= objects[i].ly;
        generated_mesh[mi].z *= objects[i].lz;
    };

    Mesh_GenerateHull(&objects[i],generated_mesh,200,nt);

    MapTexture(&objects[i],SURE_MAPPING_PLANAR_YZ);

    objects[i].movable = false;
    objects[i].collidable = true;
    objects[i].oz.x = 1;
    objects[i].oz.y = 0;
    objects[i].oz.z = 0;
    objects[i].oz = __NORMALIZE(objects[i].oz);
    objects[i].oy.x = 0;
    objects[i].oy.y = 0;
    objects[i].oy.z = 1;
    objects[i].oy = __NORMALIZE(objects[i].oy);
    objects[i].ox.x = 0;
    objects[i].ox.y = 1;
    objects[i].ox.z = 0;
    objects[i].ox = __NORMALIZE(objects[i].ox);

    ObjCoordsToDrawable(i);
    objects[i].drawable.mesh_start = objects[i].mesh_start;
    objects[i].drawable.mesh_count = objects[i].mesh_count;
    objects[i].drawable.mesh_changed = true;
    objects[i].drawable.map_id = GenTexture("tt",SURE_GENTEX_UNTRANSP);
    objects[i].drawable.type = SURE_DR_MESH; // форма
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.95; // прозрачность
    objects[i].drawable.transp_i = 0.9; // прозрачность
    objects[i].drawable.refr = 1.41; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_EQUAL; // тип рандомизации
    objects[i].drawable.dist_sigma = 0.03; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
    objects[i].drawable.rgb.s[0] = 200.0; // цвет
    objects[i].drawable.rgb.s[1] = 220.0; // цвет
    objects[i].drawable.rgb.s[2] = 255.0; // цвет
    objects[i].drawable.sided = false;
    objects[i].lp = 10;
    objects[i].initp4();
};

void SureData::Scene_tetra(double i_x,double i_y,double i_z,double i_sz,int nt,int mt,bool mvbl) // Пол и круглая лампа
{
int i;
    my_double3 generated_mesh[256];

    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = i_x; //Координаты центра
    objects[i].X.y = i_y;
    objects[i].X.z = i_z;
    objects[i].lx = i_sz; // длина
    objects[i].ly = i_sz; // ширина
    objects[i].lz = i_sz; // высота

    Mesh_GenerateTetr(&objects[i],nt);

    MapTexture(&objects[i],mt);

    objects[i].movable = mvbl;
    objects[i].collidable = true;
    objects[i].oz.x = 1;
    objects[i].oz.y = 0;
    objects[i].oz.z = 0;
    objects[i].oz = __NORMALIZE(objects[i].oz);
    objects[i].oy.x = 0;
    objects[i].oy.y = 1;
    objects[i].oy.z = 0;
    objects[i].oy = __NORMALIZE(objects[i].oy);
    objects[i].ox.x = 0;
    objects[i].ox.y = 0;
    objects[i].ox.z = 1;
    objects[i].ox = __NORMALIZE(objects[i].ox);

    ObjCoordsToDrawable(i);
    objects[i].drawable.mesh_start = objects[i].mesh_start;
    objects[i].drawable.mesh_count = objects[i].mesh_count;
    objects[i].drawable.mesh_changed = true;
    objects[i].drawable.map_id = GenTexture("tt",SURE_GENTEX_UNTRANSP);
    objects[i].drawable.type = SURE_DR_MESH; // форма
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.95; // прозрачность
    objects[i].drawable.transp_i = 0.9; // прозрачность
    objects[i].drawable.refr = 1.41; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_EQUAL; // тип рандомизации
    objects[i].drawable.dist_sigma = 0.03; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
    objects[i].drawable.rgb.s[0] = 200.0; // цвет
    objects[i].drawable.rgb.s[1] = 220.0; // цвет
    objects[i].drawable.rgb.s[2] = 255.0; // цвет
    objects[i].drawable.sided = true;
    objects[i].lp = 10;
    objects[i].initp4();
};

void SureData::Scene_cube(double i_x,double i_y,double i_z,double i_sz,int nt,int mt) // Пол и круглая лампа
{

    int i;
    my_double3 generated_mesh[256];

    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = i_x; //Координаты центра
    objects[i].X.y = i_y;
    objects[i].X.z = i_z;
    objects[i].lx = i_sz; // длина
    objects[i].ly = i_sz; // ширина
    objects[i].lz = i_sz; // высота

    Mesh_GenerateCube(&objects[i],nt);

    MapTexture(&objects[i],mt);

    objects[i].movable = false;
    objects[i].collidable = true;
    objects[i].oz.x = 1;
    objects[i].oz.y = 0;
    objects[i].oz.z = 0;
    objects[i].oz = __NORMALIZE(objects[i].oz);
    objects[i].oy.x = 0;
    objects[i].oy.y = 0;
    objects[i].oy.z = 1;
    objects[i].oy = __NORMALIZE(objects[i].oy);
    objects[i].ox.x = 0;
    objects[i].ox.y = 1;
    objects[i].ox.z = 0;
    objects[i].ox = __NORMALIZE(objects[i].ox);

    ObjCoordsToDrawable(i);
    objects[i].drawable.mesh_start = objects[i].mesh_start;
    objects[i].drawable.mesh_count = objects[i].mesh_count;
    objects[i].drawable.mesh_changed = true;
    objects[i].drawable.map_id = GenTexture("tt",SURE_GENTEX_UNTRANSP);
    objects[i].drawable.type = SURE_DR_MESH; // форма
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.95; // прозрачность
    objects[i].drawable.transp_i = 0.9; // прозрачность
    objects[i].drawable.refr = 1.41; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_EQUAL; // тип рандомизации
    objects[i].drawable.dist_sigma = 0.03; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
    objects[i].drawable.rgb.s[0] = 200.0; // цвет
    objects[i].drawable.rgb.s[1] = 220.0; // цвет
    objects[i].drawable.rgb.s[2] = 255.0; // цвет
    objects[i].drawable.sided = false;
    objects[i].lp = 10;
    objects[i].initp4();
};
*/
