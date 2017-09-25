

void SureData::Scene_box() // коробка со светящимся потолком
{
int i;
  // свет
    i = CreateObject(SURE_OBJ_PLANE);
    objects[i].X.x = 0; //Координаты центра
    objects[i].X.y = 0;
    objects[i].X.z = 100;
    objects[i].oz.z = -1;
    objects[i].lx = 50.0; // длина
    objects[i].ly = 50.0; // ширина
    objects[i].lz = 1.0; // высота
    objects[i].movable = false;
    objects[i].collidable = true;
    ObjCoordsToDrawable(i);
    objects[i].drawable.type = SURE_DR_SQUARE; // форма
    objects[i].drawable.radiance = 1.0; // свечение
    objects[i].drawable.transp = 0.0; // прозрачность
    objects[i].drawable.transp_i = 0.0; // прозрачность
    objects[i].drawable.refr = 99.0; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_EQUAL; // тип рандомизации
    objects[i].drawable.dist_sigma = 1.0; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
    objects[i].drawable.rgb.s[0] = 250; // цвет
    objects[i].drawable.rgb.s[1] = 250;
    objects[i].drawable.rgb.s[2] = 200;
    objects[i].drawable.sided = false;

    // пол
    i = CreateObject(SURE_OBJ_PLANE);
    objects[i].X.x = 0; //Координаты центра
    objects[i].X.y = 0;
    objects[i].X.z = 0;
    objects[i].lx = 50.0; // длина
    objects[i].ly = 50.0; // ширина
    objects[i].lz = 1.0; // высота
    objects[i].movable = false;
    objects[i].collidable = true;
    ObjCoordsToDrawable(i);
    objects[i].drawable.type = SURE_DR_SQUARE; // форма
    objects[i].drawable.map_id = GetTexture("parket");
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.0; // прозрачность
    objects[i].drawable.transp_i = 0.0; // прозрачность
    objects[i].drawable.refr = 1.99; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_EQUAL; // тип рандомизации
    objects[i].drawable.dist_sigma = 1.0; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
    objects[i].drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.sided = true;

    // стена +x
    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = 50; //Координаты центра
    objects[i].X.y = 0;
    objects[i].X.z = 50;
    objects[i].ox.x = 0;
    objects[i].ox.y = 0;
    objects[i].ox.z = 1;
    objects[i].oy.x = 0;
    objects[i].oy.y = 1;
    objects[i].oy.z = 0;
    objects[i].oz.x = -1;
    objects[i].oz.y = 0;
    objects[i].oz.z = 0;
    objects[i].lx = 50.0; // длина
    objects[i].ly = 50.0; // ширина
    objects[i].lz = 1.0; // высота
    objects[i].type = SURE_OBJ_PLANE;
    objects[i].movable = false;
    objects[i].collidable = true;
    ObjCoordsToDrawable(i);
    objects[i].drawable.type = SURE_DR_SQUARE; // форма
    //objects[i].drawable.map_id = GetTexture("hole");
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.0; // прозрачность
    objects[i].drawable.transp_i = 0.0; // прозрачность
    objects[i].drawable.refr = 1.1; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_NORM; // тип рандомизации
    objects[i].drawable.dist_sigma = 0.01; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
        objects[i].drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
        objects[i].drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
        objects[i].drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.sided = true;


// стена -x
    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = -50; //Координаты центра
    objects[i].X.y = 0;
    objects[i].X.z = 50;
    objects[i].ox.x = 0;
    objects[i].ox.y = 0;
    objects[i].ox.z = 1;
    objects[i].oy.x = 0;
    objects[i].oy.y = 1;
    objects[i].oy.z = 0;
    objects[i].oz.x = 1;
    objects[i].oz.y = 0;
    objects[i].oz.z = 0;
    objects[i].lx = 50.0; // длина
    objects[i].ly = 50.0; // ширина
    objects[i].lz = 1.0; // высота
    objects[i].type = SURE_OBJ_PLANE;
    objects[i].movable = false;
    objects[i].collidable = true;
    ObjCoordsToDrawable(i);
    objects[i].drawable.type = SURE_DR_SQUARE; // форма
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.0; // прозрачность
    objects[i].drawable.transp_i = 0.0; // прозрачность
    objects[i].drawable.refr = 99.0; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_NORM; // тип рандомизации
    objects[i].drawable.dist_sigma = 0.01; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
        objects[i].drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
        objects[i].drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
        objects[i].drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.sided = true;


// стена +y
    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = 0; //Координаты центра
    objects[i].X.y = 50;
    objects[i].X.z = 50;
    objects[i].ox.x = 1;
    objects[i].ox.y = 0;
    objects[i].ox.z = 0;
    objects[i].oy.x = 0;
    objects[i].oy.y = 0;
    objects[i].oy.z = 1;
    objects[i].oz.x = 0;
    objects[i].oz.y = -1;
    objects[i].oz.z = 0;
    objects[i].lx = 50.0; // длина
    objects[i].ly = 50.0; // ширина
    objects[i].lz = 1.0; // высота
    objects[i].type = SURE_OBJ_PLANE;
    objects[i].movable = false;
    objects[i].collidable = true;
    ObjCoordsToDrawable(i);
    objects[i].drawable.type = SURE_DR_SQUARE; // форма
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.0; // прозрачность
    objects[i].drawable.transp_i = 1.0; // прозрачность
    //objects[i].drawable.map_id = GetTexture("test_alpha");
    objects[i].drawable.refr = 1.01; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_NORM; // тип рандомизации
    objects[i].drawable.dist_sigma = 0.01; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
        objects[i].drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
        objects[i].drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
        objects[i].drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.sided = true;


// стена -y
    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = 0; //Координаты центра
    objects[i].X.y = -50;
    objects[i].X.z = 50;
    objects[i].ox.x = 1;
    objects[i].ox.y = 0;
    objects[i].ox.z = 0;
    objects[i].oy.x = 0;
    objects[i].oy.y = 0;
    objects[i].oy.z = 1;
    objects[i].oz.x = 0;
    objects[i].oz.y = 1;
    objects[i].oz.z = 0;
    objects[i].lx = 50.0; // длина
    objects[i].ly = 50.0; // ширина
    objects[i].lz = 1.0; // высота
    objects[i].type = SURE_OBJ_PLANE;
    objects[i].movable = false;
    objects[i].collidable = true;
    ObjCoordsToDrawable(i);
    objects[i].drawable.type = SURE_DR_SQUARE; // форма
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.0; // прозрачность
    objects[i].drawable.transp_i = 0.0; // прозрачность
    objects[i].drawable.refr = 99.0; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_NORM; // тип рандомизации
    objects[i].drawable.dist_sigma = 0.01; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
        objects[i].drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
        objects[i].drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
        objects[i].drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.sided = true;

};


void SureData::Scene_floor() // Пол и круглая лампа
{
int i;
    // пол
    i = CreateObject(SURE_OBJ_PLANE);
    objects[i].X.x = 0; //Координаты центра
    objects[i].X.y = 0;
    objects[i].X.z = 0;
    objects[i].lx = 100.0; // длина
    objects[i].ly = 100.0; // ширина
    objects[i].lz = 1.0; // высота
    objects[i].movable = false;
    objects[i].collidable = true;
    ObjCoordsToDrawable(i);
    objects[i].drawable.type = SURE_DR_SQUARE; // форма
    objects[i].drawable.map_id = GenTexture("scells",SURE_GENTEX_UNTRANSP);
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.0; // прозрачность
    objects[i].drawable.transp_i = 0.0; // прозрачность
    objects[i].drawable.refr = 1.99; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_EQUAL; // тип рандомизации
    objects[i].drawable.dist_sigma = 1.0; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
    objects[i].drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.sided = true;

  // свет
    i = CreateObject(SURE_OBJ_PLANE);
    objects[i].X.x = 60; //Координаты центра
    objects[i].X.y = 60;
    objects[i].X.z = 100;
    objects[i].oz.z = -1;
    objects[i].lx = 20.0; // длина
    objects[i].ly = 20.0; // ширина
    objects[i].lz = 1.0; // высота
    objects[i].movable = false;
    objects[i].collidable = true;
    ObjCoordsToDrawable(i);
    objects[i].drawable.type = SURE_DR_SPHERE; // форма
    objects[i].drawable.radiance = 1.0; // свечение
    objects[i].drawable.transp = 0.0; // прозрачность
    objects[i].drawable.transp_i = 0.0; // прозрачность
    objects[i].drawable.refr = 99.0; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_EQUAL; // тип рандомизации
    objects[i].drawable.dist_sigma = 1.0; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
    objects[i].drawable.rgb.s[0] = 250; // цвет
    objects[i].drawable.rgb.s[1] = 250;
    objects[i].drawable.rgb.s[2] = 200;
    objects[i].drawable.sided = false;

};


void SureData::Scene_tetrs() // Пол и лампа
{
int i;
    // пол
    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = 0; //Координаты центра
    objects[i].X.y = 0;
    objects[i].X.z = -25;
    objects[i].lx = 500.0; // длина
    objects[i].ly = 500.0; // ширина
    objects[i].lz = 100.0; // высота

    objects[i].oz.x = 0;
    objects[i].oz.y = 0;
    objects[i].oz.z = -1;
    objects[i].oz = __NORMALIZE(objects[i].oz);
    objects[i].oy.x = 1;
    objects[i].oy.y = 0;
    objects[i].oy.z = 0;
    objects[i].oy = __NORMALIZE(objects[i].oy);
    objects[i].ox.x = 0;
    objects[i].ox.y = 1;
    objects[i].ox.z = 0;
    objects[i].ox = __NORMALIZE(objects[i].ox);

    objects[i].movable = false;
    objects[i].collidable = true;
    ObjCoordsToDrawable(i);
    objects[i].drawable.type = SURE_DR_MESH; // форма

    Mesh_GenerateTetr(i,SURE_NORMALS_DEFAULT);
    MapTexture(i,SURE_MAPPING_PLANAR_XY);
    objects[i].drawable.mesh_start = objects[i].mesh_start;
    objects[i].drawable.mesh_count = objects[i].mesh_count;
    objects[i].drawable.mesh_changed = true;
    objects[i].drawable.map_id = GenTexture("scells",SURE_GENTEX_UNTRANSP);
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.0; // прозрачность
    objects[i].drawable.transp_i = 0.0; // прозрачность
    objects[i].drawable.refr = 1.99; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_EQUAL; // тип рандомизации
    objects[i].drawable.dist_sigma = 1.0; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
    objects[i].drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.sided = true;

  // свет
    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = 0; //Координаты центра
    objects[i].X.y = 0;
    objects[i].X.z = 200;
    objects[i].lx = 80.0; // длина
    objects[i].ly = 80.0; // ширина
    objects[i].lz = 10.0; // высота

    objects[i].oz.x = 0;
    objects[i].oz.y = 0;
    objects[i].oz.z = -1;
    objects[i].oz = __NORMALIZE(objects[i].oz);
    objects[i].oy.x = 1;
    objects[i].oy.y = 0;
    objects[i].oy.z = 0;
    objects[i].oy = __NORMALIZE(objects[i].oy);
    objects[i].ox.x = 0;
    objects[i].ox.y = 1;
    objects[i].ox.z = 0;
    objects[i].ox = __NORMALIZE(objects[i].ox);

    objects[i].movable = false;
    objects[i].collidable = true;

    ObjCoordsToDrawable(i);
    objects[i].drawable.type = SURE_DR_MESH; // форма
    Mesh_GenerateTetr(i,SURE_NORMALS_DEFAULT);
    MapTexture(i,SURE_MAPPING_SPHERICAL);
    objects[i].drawable.mesh_start = objects[i].mesh_start;
    objects[i].drawable.mesh_count = objects[i].mesh_count;
    objects[i].drawable.mesh_changed = true;
    objects[i].drawable.radiance = 1.0; // свечение
    objects[i].drawable.transp = 0.0; // прозрачность
    objects[i].drawable.transp_i = 0.0; // прозрачность
    objects[i].drawable.refr = 99.0; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_EQUAL; // тип рандомизации
    objects[i].drawable.dist_sigma = 1.0; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
    objects[i].drawable.rgb.s[0] = 250; // цвет
    objects[i].drawable.rgb.s[1] = 250;
    objects[i].drawable.rgb.s[2] = 200;
    objects[i].drawable.sided = false;

};

void SureData::Scene_golem() // Пол и круглая лампа
{
int i;

    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = 0; //Координаты центра
    objects[i].X.y = 0;
    objects[i].X.z = 20;
    objects[i].lx = 20.0; // длина
    objects[i].ly = 20.0; // ширина
    objects[i].lz = 20.0; // высота
    Mesh_FromFile(i,"golem");

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
    objects[i].drawable.map_id = GetTexture("golem");
    objects[i].drawable.advmap_id = GetTexture("golem_adv");
    objects[i].drawable.type = SURE_DR_MESH; // форма
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.95; // прозрачность
    objects[i].drawable.transp_i = 0.9; // прозрачность
    objects[i].drawable.refr = 1.41; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_NORM; // тип рандомизации
    objects[i].drawable.dist_sigma = 0.03; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
    objects[i].drawable.rgb.s[0] = 200.0; // цвет
    objects[i].drawable.rgb.s[1] = 220.0; // цвет
    objects[i].drawable.rgb.s[2] = 255.0; // цвет
    objects[i].drawable.sided = true;
    objects[i].lp = 10;
    objects[i].initp4();
};

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

    Mesh_GenerateHull(i,generated_mesh,200,nt);

    MapTexture(i,SURE_MAPPING_PLANAR_YZ);

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
    objects[i].drawable.sided = true;
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

    Mesh_GenerateTetr(i,nt);

    MapTexture(i,mt);

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

    Mesh_GenerateCube(i,nt);

    MapTexture(i,mt);

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
    objects[i].drawable.sided = true;
    objects[i].lp = 10;
    objects[i].initp4();
};
