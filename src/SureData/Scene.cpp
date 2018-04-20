

void SureData::Scene_box() // коробка со светящимся потолком
{
__VTYPE3 X;
  // свет
    TemplateObject.drawable.sided = true;
    TemplateObject.drawable.refr = 1.49f;
    TemplateObject.drawable.dist_type = SURE_D_EQUAL;
    TemplateObject.drawable.dist_sigma = 0.03f;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.transp = 0.0f;
    TemplateObject.drawable.transp_i = 0.1f;
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

    TemplateObject.drawable.refr = 99.0; // Коэффициент преломления
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
    TemplateObject.lx = 60.0;
    TemplateObject.ly = 60.0;
    TemplateObject.lz = 30.0;
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
    TemplateObject.drawable.refr = 1.01f;
    TemplateObject.drawable.sided = false;
    TemplateObject.collidable = false;
    TemplateObject.drawable.rgb.s[0] = 170; // цвет
    TemplateObject.drawable.rgb.s[1] = 150; // цвет
    TemplateObject.drawable.rgb.s[2] = 150; // цвет

    CreateObjectFromTemplate(&X); // пол туман
}


void SureData::Scene_floor() // Пол и круглая лампа
{
__VTYPE3 X;
    // пол
    TemplateObject.collidable = true;
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
}

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

}

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
}

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



}

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


}


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
    sprintf(TemplateObject.ModelName_drawable,"ghost_dark");
    TemplateObject.ModelID_drawable = GetModel(TemplateObject.ModelName_drawable);
    sprintf(TemplateObject.ModelName_collider,"cube");
    TemplateObject.ModelID_collider = GetModel(TemplateObject.ModelName_collider);
    TemplateObject.drawable.map_id = GetTexture("ghost_dark");//GetTexture("ghost_dark");//GenTexture("scells",SURE_GENTEX_UNTRANSP);
    //MapTexture(TemplateObject.ModelID_drawable,SURE_MAPPING_SPHERICAL);
    TemplateObject.drawable.advmap_id = -1;//GetTexture("golem_adv");
    TemplateObject.drawable.dist_type = SURE_D_EQUAL;//SURE_D_NORM;
    TemplateObject.drawable.dist_sigma = 0.005f;
    TemplateObject.drawable.rgb.s[0] = 240;
    TemplateObject.drawable.rgb.s[1] = 240;
    TemplateObject.drawable.rgb.s[2] = 253;

    TemplateObject.drawable.type = SURE_DR_MESH;
    TemplateObject.drawable.sided = false;
    TemplateObject.drawable.radiance = 0;

    CreateObjectFromTemplate(&X);
}

void SureData::Scene_Polygon()
{
    CameraInfo.cam_x.s[0] = 573;
    CameraInfo.cam_x.s[1] = -534;
    CameraInfo.cam_x.s[2] = 400;

    __VTYPE3 cx;
    cx.x = 0.7; cx.y = 0.6; cx.z = 0;
    cx = __NORMALIZE(cx);
    __VTYPE3 cv;
    cv.x = -0.6; cv.y = 0.7; cv.z = -0.36;
    cv = __NORMALIZE(cv);

    CameraInfo.cam_vec = cv;
    CameraInfo.cam_upvec = __NORMALIZE(cross(cx,cv));

    uint ScellsMapID = GenTexture("scells",SURE_GENTEX_UNTRANSP);
    __VTYPE3 X;
    TemplateObject.ox.x = 1; TemplateObject.ox.y = 0; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;

    SetTemplate_RegularCube();
    TemplateObject.drawable.map_id = ScellsMapID;
    TemplateObject.lx = 500;
    TemplateObject.ly = 500;
    TemplateObject.lz = 100;
    X.x = -500;
    X.y = 500;
    X.z = 0;
    CreateObjectFromTemplate(&X);

    TemplateObject.lx = 500;
    TemplateObject.ly = 750;
    TemplateObject.lz = 100;
    X.x = 500;
    X.y = -250;
    X.z = 0;
    CreateObjectFromTemplate(&X);

    TemplateObject.lx = 50;
    TemplateObject.ly = 750;
    TemplateObject.lz = 100;
    X.x = -950;
    X.y = -250;
    X.z = 0;
    CreateObjectFromTemplate(&X);

    TemplateObject.lx = 450;
    TemplateObject.ly = 250;
    TemplateObject.lz = 100;
    X.x = -450;
    X.y = -750;
    X.z = 0;
    CreateObjectFromTemplate(&X);

    SetTemplate_RegularCube();
    TemplateObject.drawable.rgb.s[0] = 180.0;
    TemplateObject.drawable.rgb.s[1] = 160.0;
    TemplateObject.drawable.rgb.s[2] = 250.0;
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.drawable.dist_sigma = 0.01;
    TemplateObject.lx = 60;
    TemplateObject.ly = 60;
    TemplateObject.lz = 250;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 1; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = 250;
    X.y = -500;
    X.z = 250;
    CreateObjectFromTemplate(&X);

    SetTemplate_RegularPandus();
    TemplateObject.drawable.rgb.s[0] = 190.0;
    TemplateObject.drawable.rgb.s[1] = 250.0;
    TemplateObject.drawable.rgb.s[2] = 180.0;
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.drawable.dist_sigma = 0.01;
    TemplateObject.drawable.transp = 0.93;
    TemplateObject.drawable.transp_i = 0.6;
    TemplateObject.drawable.refr = 1.21;
    TemplateObject.lx = 250;
    TemplateObject.ly = 40;
    TemplateObject.lz = 199;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = -500;
    X.y = 80;
    X.z = 300;
    CreateObjectFromTemplate(&X);

    SetTemplate_RegularPandus();
    TemplateObject.drawable.map_id = ScellsMapID;
    TemplateObject.lx = 500;
    TemplateObject.ly = 250;
    TemplateObject.lz = 100;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = 500;
    X.y = 750;
    X.z = 0;
    CreateObjectFromTemplate(&X);

    TemplateObject.lx = 250;
    TemplateObject.ly = 200;
    TemplateObject.lz = 100;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = -1; TemplateObject.oy.y = 0; TemplateObject.oy.z = 0;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = -200;
    X.y = -250;
    X.z = 0;
    CreateObjectFromTemplate(&X);

    TemplateObject.lx = 250;
    TemplateObject.ly = 200;
    TemplateObject.lz = 100;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 1; TemplateObject.oy.y = 0; TemplateObject.oy.z = 0;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = -700;
    X.y = -250;
    X.z = 0;
    CreateObjectFromTemplate(&X);

    SetTemplate_GlassCube(10);
    TemplateObject.movable = false;
    TemplateObject.lx = 249;
    TemplateObject.ly = 249;
    TemplateObject.lz = 10;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 1; TemplateObject.oz.z = 0;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 0; TemplateObject.oy.z = 1;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = 750;
    X.y = 250;
    X.z = 250;
    CreateObjectFromTemplate(&X);

    SetTemplate_EarthSphere(10.0);
    TemplateObject.lx = 150;
    TemplateObject.ly = 1;
    TemplateObject.lz = 1;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = -1;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = 750;
    X.y = -250;
    X.z = 250;
    TemplateObject.movable = false;
    CreateObjectFromTemplate(&X);

    SetTemplate_GlassSphere(10);
    TemplateObject.drawable.transp = 0;
    TemplateObject.drawable.dist_sigma = 0.03;
    TemplateObject.lx = 150;
    TemplateObject.ly = 1;
    TemplateObject.lz = 1;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = -1;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = -500;
    X.y = 500;
    X.z = 200;
    TemplateObject.movable = false;
    CreateObjectFromTemplate(&X);


    SetTemplate_RegularPlane();
    TemplateObject.lx = 80;
    TemplateObject.ly = 250;
    TemplateObject.lz = 1;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = -450;
    X.y = -250;
    X.z = -99;
    CreateObjectFromTemplate(&X);

    SetTemplate_RegularPlane();
    TemplateObject.drawable.rgb.s[0] = 250.0;
    TemplateObject.drawable.rgb.s[1] = 250.0;
    TemplateObject.drawable.rgb.s[2] = 180.0;
    TemplateObject.drawable.dist_sigma = 0.05;
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.lx = 500;
    TemplateObject.ly = 400;
    TemplateObject.lz = 1;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = -1; TemplateObject.oz.z = 0;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 0; TemplateObject.oy.z = 1;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = 500;
    X.y = 1000;
    X.z = 250;
    CreateObjectFromTemplate(&X);

    SetTemplate_RegularPlane();
    TemplateObject.drawable.map_id = GetTexture("colstones");
    TemplateObject.lx = 1000;
    TemplateObject.ly = 400;
    TemplateObject.lz = 1;
    TemplateObject.oz.x = -1; TemplateObject.oz.y = 0; TemplateObject.oz.z = 0;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 0; TemplateObject.oy.z = 1;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = 1000;
    X.y = 0;
    X.z = 250;
    CreateObjectFromTemplate(&X);

    TemplateObject.drawable.rgb.s[0] = 60.0;
    TemplateObject.drawable.rgb.s[1] = 250.0;
    TemplateObject.drawable.rgb.s[2] = 250.0;
    TemplateObject.lx = 500;
    TemplateObject.ly = 400;
    TemplateObject.lz = 1;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 1; TemplateObject.oz.z = 0;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 0; TemplateObject.oy.z = 1;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = -500;
    X.y = -1000;
    X.z = 250;
    CreateObjectFromTemplate(&X);

    TemplateObject.drawable.rgb.s[0] = 150.0;
    TemplateObject.drawable.rgb.s[1] = 150.0;
    TemplateObject.drawable.rgb.s[2] = 40.0;
    TemplateObject.lx = 1000;
    TemplateObject.ly = 400;
    TemplateObject.lz = 1;
    TemplateObject.oz.x = 1; TemplateObject.oz.y = 0; TemplateObject.oz.z = 0;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 0; TemplateObject.oy.z = 1;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = -1000;
    X.y = 0;
    X.z = 250;
    CreateObjectFromTemplate(&X);

    SetTemplate_RegularPlane();
    TemplateObject.drawable.rgb.s[0] = 250.0;
    TemplateObject.drawable.rgb.s[1] = 250.0;
    TemplateObject.drawable.rgb.s[2] = 180.0;
    TemplateObject.drawable.dist_sigma = 0.05;
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.lx = 700;
    TemplateObject.ly = 400;
    TemplateObject.lz = 1;
    TemplateObject.oz.x = 0.3; TemplateObject.oz.y = -1; TemplateObject.oz.z = 0;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 0; TemplateObject.oy.z = 1;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = -500;
    X.y = 800;
    X.z = 250;
    CreateObjectFromTemplate(&X);

    SetTemplate_RegularPlane();
    TemplateObject.drawable.rgb.s[0] = 200.0;
    TemplateObject.drawable.rgb.s[1] = 250.0;
    TemplateObject.drawable.rgb.s[2] = 180.0;
    TemplateObject.lx = 700;
    TemplateObject.ly = 400;
    TemplateObject.lz = 1;
    TemplateObject.oz.x = -0.3; TemplateObject.oz.y = 1; TemplateObject.oz.z = 0;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 0; TemplateObject.oy.z = 1;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = 500;
    X.y = -800;
    X.z = 250;
    CreateObjectFromTemplate(&X);

    SetTemplate_FogPlane();
    TemplateObject.lx = 1010;
    TemplateObject.ly = 1010;
    TemplateObject.lz = 250;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    X.x = 0;
    X.y = 0;
    X.z = 80;
    CreateObjectFromTemplate(&X);

    SetTemplate_GlowPlane();
    TemplateObject.ox.x = 0; TemplateObject.ox.y = 1; TemplateObject.ox.z = 0;
    TemplateObject.oy.x = 1; TemplateObject.oy.y = 0; TemplateObject.oy.z = 0;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = -1;
    TemplateObject.lx = 1000;
    TemplateObject.ly = 1000;
    TemplateObject.lz = 1;
    X.x = 0;
    X.y = 0;
    X.z = 500;
    CreateObjectFromTemplate(&X);

    // 360, -128
    SetTemplate_RegularPandus();
    TemplateObject.lx = 100;
    TemplateObject.ly = 100;
    TemplateObject.lz = 100;
    TemplateObject.lp = 170;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oz,TemplateObject.oy));
    TemplateObject.type = SURE_OBJ_NONE;
    TemplateObject.drawable.type = SURE_DR_NONE;
    TemplateObject.collidable = true;
    TemplateObject.movable = true;
    X.x = 360;
    X.y = -130;
    X.z = 100+200+20+50+10;
    TemplateObject.mass = 20.0;
    uint ParentCup = CreateObjectFromTemplate(&X);

    SetTemplate_RegularPandus();

    uint ScellsMapID2 = GenTexture("scells",SURE_GENTEX_UNTRANSP);
    TemplateObject.drawable.map_id = ScellsMapID2;

    TemplateObject.lx = 100;
    TemplateObject.ly = 50;
    TemplateObject.lz = 50;
    TemplateObject.collidable = true;
    TemplateObject.movable = true;
    TemplateObject.ox_byparent.x = 1; TemplateObject.ox_byparent.y = 0; TemplateObject.ox_byparent.z = 0;
    TemplateObject.oy_byparent.x = 0; TemplateObject.oy_byparent.y = 1; TemplateObject.oy_byparent.z = 0;
    TemplateObject.X_byparent.x = 0;
    TemplateObject.X_byparent.y = -51;
    TemplateObject.X_byparent.z = 0;
    TemplateObject.mass = 20.0;
    TemplateObject.ParentID = ParentCup;
    CreateObjectFromTemplate(&X);

    TemplateObject.lx = 100;
    TemplateObject.ly = 50;
    TemplateObject.lz = 50;
    TemplateObject.collidable = true;
    TemplateObject.movable = true;
    TemplateObject.ox_byparent.x = -1; TemplateObject.ox_byparent.y = 0; TemplateObject.ox_byparent.z = 0;
    TemplateObject.oy_byparent.x = 0; TemplateObject.oy_byparent.y = -1; TemplateObject.oy_byparent.z = 0;
    TemplateObject.X_byparent.x = 0;
    TemplateObject.X_byparent.y = 51;
    TemplateObject.X_byparent.z = 0;
    TemplateObject.mass = 20.0;
    TemplateObject.ParentID = ParentCup;
    CreateObjectFromTemplate(&X);

    TemplateObject.lx = 100;
    TemplateObject.ly = 50;
    TemplateObject.lz = 50;
    TemplateObject.collidable = true;
    TemplateObject.movable = true;
    TemplateObject.ox_byparent.x = 0; TemplateObject.ox_byparent.y = -1; TemplateObject.ox_byparent.z = 0;
    TemplateObject.oy_byparent.x = 1; TemplateObject.oy_byparent.y = 0; TemplateObject.oy_byparent.z = 0;
    TemplateObject.X_byparent.x = -51;
    TemplateObject.X_byparent.y = 0;
    TemplateObject.X_byparent.z = 0;
    TemplateObject.mass = 20.0;
    TemplateObject.ParentID = ParentCup;
    CreateObjectFromTemplate(&X);

    TemplateObject.lx = 100;
    TemplateObject.ly = 50;
    TemplateObject.lz = 50;
    TemplateObject.collidable = true;
    TemplateObject.movable = true;
    TemplateObject.ox_byparent.x = 0; TemplateObject.ox_byparent.y = 1; TemplateObject.ox_byparent.z = 0;
    TemplateObject.oy_byparent.x = -1; TemplateObject.oy_byparent.y = 0; TemplateObject.oy_byparent.z = 0;
    TemplateObject.X_byparent.x = 51;
    TemplateObject.X_byparent.y = 0;
    TemplateObject.X_byparent.z = 0;
    TemplateObject.mass = 20.0;
    TemplateObject.ParentID = ParentCup;
    CreateObjectFromTemplate(&X);

    // 360, -128
    // Тут будет стол
    /*
    SetTemplate_RegularCube();
    TemplateObject.lx = 200;
    TemplateObject.ly = 200;
    TemplateObject.lz = 200;
    TemplateObject.lp = 450;
    TemplateObject.oz.x = 0; TemplateObject.oz.y = 0; TemplateObject.oz.z = 1;
    TemplateObject.oz = __NORMALIZE(TemplateObject.oz);
    TemplateObject.oy.x = 0; TemplateObject.oy.y = 1; TemplateObject.oy.z = 0;
    TemplateObject.oy = __NORMALIZE(TemplateObject.oy);
    TemplateObject.ox = __NORMALIZE(cross(TemplateObject.oy,TemplateObject.oz));
    TemplateObject.type = SURE_OBJ_NONE;
    TemplateObject.drawable.type = SURE_DR_NONE;
    TemplateObject.collidable = true;
    TemplateObject.movable = true;
    X.x = 360;
    X.y = -130;
    X.z = 100+100+10;
    TemplateObject.mass = 200.0;
    uint ParentTable = CreateObjectFromTemplate(&X);

    SetTemplate_RegularCube();

    TemplateObject.lx = 160;
    TemplateObject.ly = 160;
    TemplateObject.lz = 10;
    TemplateObject.collidable = true;
    TemplateObject.movable = true;
    TemplateObject.ox_byparent.x = 1; TemplateObject.ox_byparent.y = 0; TemplateObject.ox_byparent.z = 0;
    TemplateObject.oy_byparent.x = 0; TemplateObject.oy_byparent.y = 1; TemplateObject.oy_byparent.z = 0;
    TemplateObject.X_byparent.x = 0;
    TemplateObject.X_byparent.y = 0;
    TemplateObject.X_byparent.z = 95;
    TemplateObject.mass = 200.0;
    TemplateObject.ParentID = ParentTable;
    CreateObjectFromTemplate(&X);

    TemplateObject.lx = 20;
    TemplateObject.ly = 20;
    TemplateObject.lz = 100;
    TemplateObject.collidable = true;
    TemplateObject.movable = true;
    TemplateObject.ox_byparent.x = 1; TemplateObject.ox_byparent.y = 0; TemplateObject.ox_byparent.z = 0;
    TemplateObject.oy_byparent.x = 0; TemplateObject.oy_byparent.y = 1; TemplateObject.oy_byparent.z = 0;
    TemplateObject.X_byparent.x = -130;
    TemplateObject.X_byparent.y = -130;
    TemplateObject.X_byparent.z = 0;
    TemplateObject.mass = 200.0;
    TemplateObject.ParentID = ParentTable;
    CreateObjectFromTemplate(&X);

    TemplateObject.lx = 20;
    TemplateObject.ly = 20;
    TemplateObject.lz = 100;
    TemplateObject.collidable = true;
    TemplateObject.movable = true;
    TemplateObject.ox_byparent.x = 1; TemplateObject.ox_byparent.y = 0; TemplateObject.ox_byparent.z = 0;
    TemplateObject.oy_byparent.x = 0; TemplateObject.oy_byparent.y = 1; TemplateObject.oy_byparent.z = 0;
    TemplateObject.X_byparent.x = -130;
    TemplateObject.X_byparent.y = 130;
    TemplateObject.X_byparent.z = 0;
    TemplateObject.mass = 200.0;
    TemplateObject.ParentID = ParentTable;
    CreateObjectFromTemplate(&X);

    TemplateObject.lx = 20;
    TemplateObject.ly = 20;
    TemplateObject.lz = 100;
    TemplateObject.collidable = true;
    TemplateObject.movable = true;
    TemplateObject.ox_byparent.x = 1; TemplateObject.ox_byparent.y = 0; TemplateObject.ox_byparent.z = 0;
    TemplateObject.oy_byparent.x = 0; TemplateObject.oy_byparent.y = 1; TemplateObject.oy_byparent.z = 0;
    TemplateObject.X_byparent.x = 130;
    TemplateObject.X_byparent.y = 130;
    TemplateObject.X_byparent.z = 0;
    TemplateObject.mass = 200.0;
    TemplateObject.ParentID = ParentTable;
    CreateObjectFromTemplate(&X);

    TemplateObject.lx = 20;
    TemplateObject.ly = 20;
    TemplateObject.lz = 100;
    TemplateObject.collidable = true;
    TemplateObject.movable = true;
    TemplateObject.ox_byparent.x = 1; TemplateObject.ox_byparent.y = 0; TemplateObject.ox_byparent.z = 0;
    TemplateObject.oy_byparent.x = 0; TemplateObject.oy_byparent.y = 1; TemplateObject.oy_byparent.z = 0;
    TemplateObject.X_byparent.x = 130;
    TemplateObject.X_byparent.y = -130;
    TemplateObject.X_byparent.z = 0;
    TemplateObject.mass = 200.0;
    TemplateObject.ParentID = ParentTable;
    CreateObjectFromTemplate(&X);
    */
    LoadObjectFromFile("table");
    //SaveObjectToFile(ObjByID(ParentTable),"table");

    SetTemplate_RegularTetr(1.0);
    TemplateObject.lx = 300;
    TemplateObject.ly = 200;
    TemplateObject.lz = 150;
    TemplateObject.collidable = true;
    TemplateObject.movable = true;
    TemplateObject.mass = 0.1;
    X.x = 200;
    X.y = 200;
    X.z = 250;
    uint ps = CreateObjectFromTemplate(&X);
    ObjByID(ps)->type = SURE_OBJ_PS;
    ObjByID(ps)->PSSparks = true;
    SetParticlesNumber(ObjByID(ps),500);

}
