void SureData::Mesh_GenerateTetr(int i_model,int norm_type)
{
    ModelsInfo[i_model].mesh_start = cur_meshes;
    ModelsInfo[i_model].vertex_start = cur_vertexes;
    my_double3 p1,p2,p3,p4;

    p1.x = SURE_P4_X;
    p1.y = 0;
    p1.z = -SURE_P4_Y;
    p2.x = -SURE_P4_Y;
    p2.y = 0.5;
    p2.z = -SURE_P4_Y;
    p3.x = -SURE_P4_Y;
    p3.y = -0.5;
    p3.z = -SURE_P4_Y;
    p4.x = 0;
    p4.y = 0;
    p4.z = SURE_P4_Z;

    int cv = AddVertex(p1); // cv
    AddVertex(p2);
    AddVertex(p3);
    AddVertex(p4);

    AddMesh(cv  ,cv+2,cv+1); // нижняя грань
    AddMesh(cv  ,cv+3,cv+2); // +x +y
    AddMesh(cv  ,cv+1,cv+3); // +x -y
    AddMesh(cv+1,cv+2,cv+3); // -x
    ModelsInfo[i_model].mesh_count = 4;
    ModelsInfo[i_model].vertex_count = cur_vertexes - ModelsInfo[i_model].vertex_start;

    Mesh_GenNormals(i_model,norm_type);

}

void SureData::Mesh_GenNormals(int i_model, int norm_type)
{
for(uint ms = 0; ms<ModelsInfo[i_model].mesh_count; ++ms)
    {
        int im = ModelsInfo[i_model].mesh_start + ms;
        my_double3 v1;
        v1.x = __VERTEX_X(__MESH_V1(im));
        v1.y = __VERTEX_Y(__MESH_V1(im));
        v1.z = __VERTEX_Z(__MESH_V1(im));
        my_double3 v2;
        v2.x = __VERTEX_X(__MESH_V2(im));
        v2.y = __VERTEX_Y(__MESH_V2(im));
        v2.z = __VERTEX_Z(__MESH_V2(im));
        my_double3 v3;
        v3.x = __VERTEX_X(__MESH_V3(im));
        v3.y = __VERTEX_Y(__MESH_V3(im));
        v3.z = __VERTEX_Z(__MESH_V3(im));
        if(norm_type==SURE_NORMALS_DEFAULT)
        {
            my_double3 n = __NORMALIZE(cross(v2-v1,v3-v1));
            __NORMAL1_X(im) = n.x;
            __NORMAL1_Y(im) = n.y;
            __NORMAL1_Z(im) = n.z;
            __NORMAL2_X(im) = n.x;
            __NORMAL2_Y(im) = n.y;
            __NORMAL2_Z(im) = n.z;
            __NORMAL3_X(im) = n.x;
            __NORMAL3_Y(im) = n.y;
            __NORMAL3_Z(im) = n.z;
        };
        if(norm_type==SURE_NORMALS_SHPERICAL)
        {
            my_double3 n1 = __NORMALIZE(v1);
            my_double3 n2 = __NORMALIZE(v2);
            my_double3 n3 = __NORMALIZE(v3);
            __NORMAL1_X(im) = n1.x;
            __NORMAL1_Y(im) = n1.y;
            __NORMAL1_Z(im) = n1.z;
            __NORMAL2_X(im) = n2.x;
            __NORMAL2_Y(im) = n2.y;
            __NORMAL2_Z(im) = n2.z;
            __NORMAL3_X(im) = n3.x;
            __NORMAL3_Y(im) = n3.y;
            __NORMAL3_Z(im) = n3.z;
        };
    };

}

void SureData::Mesh_GenerateCube(int i_model,int norm_type)
{
    ModelsInfo[i_model].mesh_start = cur_meshes;
    ModelsInfo[i_model].vertex_start = cur_vertexes;
    double x = 1.0f;
    double y = 1.0f;
    double z = 1.0f;
    int cv = AddVertex(-x,-y,-z); // cv
    AddVertex(-x, y,-z); // cv+1
    AddVertex( x, y,-z); // cv+2
    AddVertex( x,-y,-z); // cv+3
    AddVertex( x,-y, z); // cv+4
    AddVertex(-x,-y, z); // cv+5
    AddVertex(-x, y, z); // cv+6
    AddVertex( x, y, z); // cv+7
    AddMesh(cv  ,cv+1,cv+2);
    AddMesh(cv+2,cv+3,cv  );
    AddMesh(cv+4,cv+6,cv+5);
    AddMesh(cv+6,cv+4,cv+7);
    AddMesh(cv  ,cv+6,cv+1);
    AddMesh(cv+6,cv  ,cv+5);
    AddMesh(cv+1,cv+7,cv+2);
    AddMesh(cv+7,cv+1,cv+6);
    AddMesh(cv+2,cv+4,cv+3);
    AddMesh(cv+4,cv+2,cv+7);
    AddMesh(cv+3,cv+5,cv  );
    AddMesh(cv+5,cv+3,cv+4);
    ModelsInfo[i_model].mesh_count = 12;
    ModelsInfo[i_model].vertex_count = cur_vertexes - ModelsInfo[i_model].vertex_start;

    Mesh_GenNormals(i_model,norm_type);

}

void SureData::Mesh_GeneratePandus(int i_model,int norm_type)
{
    ModelsInfo[i_model].mesh_start = cur_meshes;
    ModelsInfo[i_model].vertex_start = cur_vertexes;
    double x = 1.0f;
    double y = 1.0f;
    double z = 1.0f;
    int cv = AddVertex(-x,-y,-z); // cv
    AddVertex(-x, y,-z); // cv+1
    AddVertex( x, y,-z); // cv+2
    AddVertex( x,-y,-z); // cv+3
    AddVertex( x,-y, z); // cv+4
    AddVertex(-x,-y, z); // cv+5
    //AddVertex(-x, y, z); // cv+6
    //AddVertex( x, y, z); // cv+7
    AddMesh(cv  ,cv+1,cv+2);
    AddMesh(cv+2,cv+3,cv  );
    //AddMesh(cv+4,cv+6,cv+5);
    //AddMesh(cv+6,cv+4,cv+7);
    //AddMesh(cv  ,cv+6,cv+1);
    AddMesh(cv  ,cv+5,cv+1);
    //AddMesh(cv+6,cv  ,cv+5);
    //AddMesh(cv+1,cv+7,cv+2);
    //AddMesh(cv+7,cv+1,cv+6);
    //AddMesh(cv+2,cv+4,cv+3);
    AddMesh(cv+2,cv+4,cv+3);
    //AddMesh(cv+4,cv+2,cv+7);
    AddMesh(cv+3,cv+5,cv  );
    AddMesh(cv+5,cv+3,cv+4);

    AddMesh(cv+1,cv+4,cv+2);
    AddMesh(cv+4,cv+1,cv+5);

    ModelsInfo[i_model].mesh_count = 8;
    ModelsInfo[i_model].vertex_count = cur_vertexes - ModelsInfo[i_model].vertex_start;

    Mesh_GenNormals(i_model,norm_type);

}

void SureData::Mesh_GenerateHull(int i_model,my_double3* vertexes,int vert_count,int norm_type)
{
    ModelsInfo[i_model].mesh_start = cur_meshes;

    int cover[400][3];
    int tcover[400][3];

    double min_za = SURE_R_MAXDISTANCE;
    double min_zb = SURE_R_MAXDISTANCE;
    double max_x = -SURE_R_MAXDISTANCE;
    double max_z = -SURE_R_MAXDISTANCE;
    int min_zai = 0;
    int min_zbi = 0;
    int max_xi = 0;
    int max_zi = 0;

    for(int mi = 0;mi<vert_count;++mi)
    {
        if(vertexes[mi].z>max_z){max_zi=mi;max_z =vertexes[mi].z;};
        if(vertexes[mi].x>max_x&&vertexes[mi].z<0){max_xi=mi;max_x=vertexes[mi].x;};
        if(vertexes[mi].x<0&&vertexes[mi].y<0&&vertexes[mi].z<min_za){min_zai=mi;min_za=vertexes[mi].z;};
        if(vertexes[mi].x<0&&vertexes[mi].y>0&&vertexes[mi].z<min_zb){min_zbi=mi;min_zb=vertexes[mi].z;};
    };

    int cover_c = 0;
    int tcover_c = 0;
    cover[cover_c][0] = max_zi;
    cover[cover_c][1] = min_zai;
    cover[cover_c][2] = max_xi;
    cover_c++;
    cover[cover_c][0] = max_zi;
    cover[cover_c][1] = max_xi;
    cover[cover_c][2] = min_zbi;
    cover_c++;
    cover[cover_c][0] = max_zi;
    cover[cover_c][1] = min_zbi;
    cover[cover_c][2] = min_zai;
    cover_c++;
    cover[cover_c][0] = min_zai;
    cover[cover_c][1] = min_zbi;
    cover[cover_c][2] = max_xi;
    cover_c++;

    for(int i = 0;i<vert_count;++i)
    {
        bool outside = false;
        for(int icc=0;icc<cover_c;++icc)
        {
            my_double3 n = cross(vertexes[cover[icc][1]]-vertexes[cover[icc][0]],
                                 vertexes[cover[icc][2]]-vertexes[cover[icc][0]]);
            double d = dot(n,vertexes[i]-vertexes[cover[icc][0]]);
            if(d>SURE_R_DELTA)outside=true;
        };
        if(outside)
        {
            for(int icc=0;icc<cover_c;++icc)
            { // для всех граней cover
                my_double3 n = cross(vertexes[cover[icc][1]]-vertexes[cover[icc][0]],
                                     vertexes[cover[icc][2]]-vertexes[cover[icc][0]]);
                double d = dot(n,vertexes[i]-vertexes[cover[icc][0]]);
                if(d<0)
                { // cover -> tcover все невидимые грани
                    tcover[tcover_c][0] = cover[icc][0];
                    tcover[tcover_c][1] = cover[icc][1];
                    tcover[tcover_c][2] = cover[icc][2];
                    tcover_c++;
                }else
                { // если грань видимая -- то для каждого ребра:
                    bool inside;
                    tcover[tcover_c][0] = cover[icc][0];
                    tcover[tcover_c][1] = cover[icc][1];
                    tcover[tcover_c][2] = i;
                    // строим грань с новой точкой и проверяем все точки cover
                    n = cross(vertexes[tcover[tcover_c][1]]-vertexes[tcover[tcover_c][0]],
                              vertexes[tcover[tcover_c][2]]-vertexes[tcover[tcover_c][0]]);
                    inside = true;
                    for(int ic = 0;ic<cover_c;++ic)for(int icp=0;icp<3;++icp)
                    {   d = dot(n,vertexes[cover[ic][icp]]-vertexes[tcover[tcover_c][0]]);
                        if(d>SURE_P_DELTA)
                      { inside = false; ic = cover_c; icp = 3; }; };
                    // если все внутри -- грань в cover
                    if(inside)
                    { ++tcover_c; };

                    tcover[tcover_c][0] = cover[icc][1];
                    tcover[tcover_c][1] = cover[icc][2];
                    tcover[tcover_c][2] = i;
                    // строим грань с новой точкой и проверяем все точки cover
                    n = cross(vertexes[tcover[tcover_c][1]]-vertexes[tcover[tcover_c][0]],
                              vertexes[tcover[tcover_c][2]]-vertexes[tcover[tcover_c][0]]);
                    inside = true;
                    for(int ic = 0;ic<cover_c;++ic)for(int icp=0;icp<3;++icp)
                    {   d = dot(n,vertexes[cover[ic][icp]]-vertexes[tcover[tcover_c][0]]);
                        if(d>SURE_P_DELTA)
                      { inside = false; ic = cover_c; icp = 3; }; };
                    // если все внутри -- грань в cover
                    if(inside)
                    { ++tcover_c; };

                    tcover[tcover_c][0] = cover[icc][2];
                    tcover[tcover_c][1] = cover[icc][0];
                    tcover[tcover_c][2] = i;
                    // строим грань с новой точкой и проверяем все точки cover
                    n = cross(vertexes[tcover[tcover_c][1]]-vertexes[tcover[tcover_c][0]],
                              vertexes[tcover[tcover_c][2]]-vertexes[tcover[tcover_c][0]]);
                    inside = true;
                    for(int ic = 0;ic<cover_c;++ic)for(int icp=0;icp<3;++icp)
                    {   d = dot(n,vertexes[cover[ic][icp]]-vertexes[tcover[tcover_c][0]]);
                        if(d>SURE_P_DELTA)
                      { inside = false; ic = cover_c; icp = 3; }; };
                    // если все внутри -- грань в cover
                    if(inside)
                    { ++tcover_c; };
                };
            };    // для всех граней cover
            for(int ic = 0;ic<tcover_c;++ic)for(int icp=0;icp<3;++icp)
            {
                cover[ic][icp] = tcover[ic][icp];
            };
            cover_c = tcover_c;
            tcover_c = 0;
        };
    };

    ModelsInfo[i_model].mesh_count = 0;
    for(int ic = 0;ic<cover_c;++ic)
    {
        int z = AddMesh(AddVertex(vertexes[cover[ic][0]]),
                AddVertex(vertexes[cover[ic][1]]),
                AddVertex(vertexes[cover[ic][2]]));

                if(norm_type==SURE_NORMALS_DEFAULT)
                {
                my_double3 n = __NORMALIZE(cross(vertexes[cover[ic][1]]-vertexes[cover[ic][0]],vertexes[cover[ic][2]]-vertexes[cover[ic][0]]));
                    __NORMAL1_X(z) = n.x;
                    __NORMAL1_Y(z) = n.y;
                    __NORMAL1_Z(z) = n.z;
                    __NORMAL2_X(z) = n.x;
                    __NORMAL2_Y(z) = n.y;
                    __NORMAL2_Z(z) = n.z;
                    __NORMAL3_X(z) = n.x;
                    __NORMAL3_Y(z) = n.y;
                    __NORMAL3_Z(z) = n.z;
                };
                if(norm_type==SURE_NORMALS_SHPERICAL)
                {
                my_double3 n1 = __NORMALIZE(vertexes[cover[ic][0]]);
                my_double3 n2 = __NORMALIZE(vertexes[cover[ic][1]]);
                my_double3 n3 = __NORMALIZE(vertexes[cover[ic][2]]);
                    __NORMAL1_X(z) = n1.x;
                    __NORMAL1_Y(z) = n1.y;
                    __NORMAL1_Z(z) = n1.z;
                    __NORMAL2_X(z) = n2.x;
                    __NORMAL2_Y(z) = n2.y;
                    __NORMAL2_Z(z) = n2.z;
                    __NORMAL3_X(z) = n3.x;
                    __NORMAL3_Y(z) = n3.y;
                    __NORMAL3_Z(z) = n3.z;
                };
        ModelsInfo[i_model].mesh_count++;
    };
}

int SureData::GenModel(const char* name,int type)
{
int result = cur_models;
if(type == 0)
    Mesh_GenerateCube(cur_models,SURE_NORMALS_DEFAULT);
if(type == 1)
    Mesh_GenerateTetr(cur_models,SURE_NORMALS_DEFAULT);
if(type == 2)
    Mesh_GeneratePandus(cur_models,SURE_NORMALS_DEFAULT);
MapTexture(cur_models,SURE_MAPPING_PLANAR_XY);
sprintf(ModelsInfo[cur_models].name,"%s",name);
ModelsInfo[cur_models].toupdate = true;
cur_models++;
return result;
}

int SureData::GenTexture(const char* name,int type)
{
            float rndcolors[8];
            for(int k = 0;k<6;++k){
                rndcolors[k] = 255.0*((float)rand()/(float)RAND_MAX);
            };
        if(type==SURE_GENTEX_UNTRANSP)
        {
            rndcolors[3] = 255.0;
            rndcolors[7] = 255.0;
        };
        if(type==SURE_GENTEX_HALFTRANSP)
        {
            rndcolors[3] = 0.0;
            rndcolors[7] = 255.0;
        };
        cl_uchar* tex = &TexturesData[cur_textures*SURE_R_TEXRES*SURE_R_TEXRES*4];
        for(int iy = 0;iy<SURE_R_TEXRES;++iy)
        for(int ix = 0;ix<SURE_R_TEXRES;++ix)
        {
            int cx = ix>>5;
            int cy = iy>>5;
            if(((cx%2==1)&&(cy%2==1))||
                ((cx%2==0)&&(cy%2==0)))
            {
                *tex = rndcolors[0]; ++tex;
                *tex = rndcolors[1]; ++tex;
                *tex = rndcolors[2]; ++tex;
                *tex = rndcolors[3]; ++tex;
            }else{
                *tex = rndcolors[4]; ++tex;
                *tex = rndcolors[5]; ++tex;
                *tex = rndcolors[6]; ++tex;
                *tex = rndcolors[7]; ++tex;
            };
        };
        TexturesInfo[cur_textures].toupdate = true;
        sprintf(TexturesInfo[cur_textures].name,"%s%d",name,cur_textures);
        cur_textures++;
        return cur_textures - 1;

}

void SureData::SetNextTemplate(){
    switch (TemplateIndex){
    case 0:
    {
        SetTemplate_DarkCube(10.0f);
        break;
    };
    case 1:
    {
        SetTemplate_GlassCube(10.0f);
        break;
    };
    case 2:
    {
        SetTemplate_DarkSphere(10.0f);
        break;
    };
    case 3:
    {
        SetTemplate_GlassSphere(10.0f);
        break;
    };
    case 4:
    {
        SetTemplate_EarthSphere(10.0f);
        break;
    };
    case 5:
    {
        SetTemplate_EarthCube(10.0f);
        break;
    };
    case 6:
    {
        SetTemplate_GlowCube(10.0f);
        break;
    };
     case 7:
    {
        SetTemplate_GlowSphere(10.0f);
        break;
    };
     case 8:
    {
        SetTemplate_RegularTetr(20.0f);
        break;
    };
    default:
        break;
    };
    clock_gettime(CLOCK_MONOTONIC,&ShowTemplateTime);
    ShowTemplate = true;
    TemplateIndex++;
    if(TemplateIndex>=Templates)
        TemplateIndex = 0;
}

void SureData::SetTemplate_GlassSphere(float Radius){
    TemplateObject.ParentID = -1;
    TemplateObject.drawable.rgb.s[0] = 240.0;
    TemplateObject.drawable.rgb.s[1] = 240.0;
    TemplateObject.drawable.rgb.s[2] = 250.0;
    TemplateObject.movable = true;
    TemplateObject.collidable = true;
    TemplateObject.drawable.sided = false;
    TemplateObject.type = SURE_OBJ_SPHERE;
    TemplateObject.drawable.radiance = 0;
    TemplateObject.drawable.type = SURE_DR_SPHERE; // форма
    TemplateObject.lx = Radius;
    TemplateObject.lp = Radius*1.7;
    TemplateObject.drawable.transp = 0.96;
    TemplateObject.drawable.refr = 1.21f;
    TemplateObject.drawable.transp_i = 0.88;
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.dist_sigma = 0.01f;
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
}

void SureData::SetTemplate_EarthSphere(float Radius){
TemplateObject.ParentID = -1;
    TemplateObject.drawable.rgb.s[0] = 240.0;
    TemplateObject.drawable.rgb.s[1] = 240.0;
    TemplateObject.drawable.rgb.s[2] = 250.0;
    TemplateObject.movable = true;
    TemplateObject.collidable = true;
    TemplateObject.drawable.sided = false;
    TemplateObject.type = SURE_OBJ_SPHERE;
    TemplateObject.drawable.radiance = 0;
    TemplateObject.drawable.type = SURE_DR_SPHERE; // форма
    TemplateObject.lx = Radius;
    TemplateObject.lp = Radius*1.7;
    TemplateObject.drawable.transp = 0.96;
    TemplateObject.drawable.refr = 1.21f;
    TemplateObject.drawable.transp_i = 0.88;
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.dist_sigma = 0.01f;
    TemplateObject.drawable.map_id = GetTexture("earth");
    TemplateObject.drawable.advmap_id = GetTexture("earth_adv");
}

void SureData::SetTemplate_DarkSphere(float Radius){
TemplateObject.ParentID = -1;
    TemplateObject.drawable.rgb.s[0] = 13.0;
    TemplateObject.drawable.rgb.s[1] = 10.0;
    TemplateObject.drawable.rgb.s[2] = 10.0;
    TemplateObject.movable = true;
    TemplateObject.collidable = true;
    TemplateObject.drawable.sided = false;
    TemplateObject.type = SURE_OBJ_SPHERE;
    TemplateObject.drawable.radiance = 0;
    TemplateObject.drawable.type = SURE_DR_SPHERE; // форма
    TemplateObject.lx = Radius;
    TemplateObject.lp = Radius*1.7;
    TemplateObject.drawable.transp = 1.1f;
    TemplateObject.drawable.transp_i = 0.002f;
    TemplateObject.drawable.refr = 1.02;
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.dist_sigma = 0.01f;
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
}

void SureData::SetTemplate_DarkCube(float SideLen){
TemplateObject.ParentID = -1;
    TemplateObject.movable = true;
    TemplateObject.collidable = true;
    TemplateObject.drawable.sided = false;
    TemplateObject.drawable.rgb.s[0] = 13.0;
    TemplateObject.drawable.rgb.s[1] = 10.0;
    TemplateObject.drawable.rgb.s[2] = 10.0;
    TemplateObject.type = SURE_OBJ_MESH;
    TemplateObject.lx = SideLen; // длина
    TemplateObject.ly = SideLen; // ширина
    TemplateObject.lz = SideLen; // высота
    TemplateObject.lp = SideLen*1.7f;
    TemplateObject.drawable.radiance = 0.0f;
    sprintf(TemplateObject.ModelName_drawable,"cube");
    TemplateObject.ModelID_drawable = GetModel(TemplateObject.ModelName_drawable);
    sprintf(TemplateObject.ModelName_collider,"cube");
    TemplateObject.ModelID_collider = GetModel(TemplateObject.ModelName_collider);
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
    TemplateObject.drawable.transp = 1.1f;
    TemplateObject.drawable.transp_i = 0.002f;
    TemplateObject.drawable.refr = 1.02;
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.drawable.dist_sigma = 0.01f;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.type = SURE_DR_MESH;
}

void SureData::SetTemplate_GlassCube(float SideLen){
TemplateObject.ParentID = -1;
    TemplateObject.movable = true;
    TemplateObject.collidable = true;
    TemplateObject.drawable.sided = false;
    TemplateObject.drawable.rgb.s[0] = 240.0;
    TemplateObject.drawable.rgb.s[1] = 240.0;
    TemplateObject.drawable.rgb.s[2] = 250.0;
    TemplateObject.type = SURE_OBJ_MESH;
    TemplateObject.lx = SideLen; // длина
    TemplateObject.ly = SideLen; // ширина
    TemplateObject.lz = SideLen * 0.05f; // высота
    TemplateObject.lp = SideLen*1.7f;
    TemplateObject.drawable.radiance = 0.0f;
    sprintf(TemplateObject.ModelName_drawable,"cube");
    TemplateObject.ModelID_drawable = GetModel(TemplateObject.ModelName_drawable);
    sprintf(TemplateObject.ModelName_collider,"cube");
    TemplateObject.ModelID_collider = GetModel(TemplateObject.ModelName_collider);
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
    TemplateObject.drawable.transp = 0.96;
    TemplateObject.drawable.refr = 1.21f;
    TemplateObject.drawable.transp_i = 0.88;
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.dist_sigma = 0.01f;
    TemplateObject.drawable.type = SURE_DR_MESH;
}

void SureData::SetTemplate_EarthCube(float SideLen){
TemplateObject.ParentID = -1;
    TemplateObject.movable = true;
    TemplateObject.drawable.sided = false;
    TemplateObject.collidable = true;
    TemplateObject.drawable.rgb.s[0] = 13.0;
    TemplateObject.drawable.rgb.s[1] = 10.0;
    TemplateObject.drawable.rgb.s[2] = 10.0;
    TemplateObject.type = SURE_OBJ_MESH;
    TemplateObject.lx = SideLen; // длина
    TemplateObject.ly = SideLen; // ширина
    TemplateObject.lz = SideLen; // высота
    TemplateObject.lp = SideLen*1.7f;
    TemplateObject.drawable.radiance = 0.0f;
    sprintf(TemplateObject.ModelName_drawable,"cube");
    TemplateObject.ModelID_drawable = GetModel(TemplateObject.ModelName_drawable);
    sprintf(TemplateObject.ModelName_collider,"cube");
    TemplateObject.ModelID_collider = GetModel(TemplateObject.ModelName_collider);
    TemplateObject.drawable.map_id = GetTexture("earth");
    TemplateObject.drawable.advmap_id = GetTexture("earth_adv");
    MapTexture(TemplateObject.ModelID_drawable,SURE_MAPPING_SPHERICAL);
    TemplateObject.drawable.transp = 1.1f;
    TemplateObject.drawable.transp_i = 0.002f;
    TemplateObject.drawable.refr = 1.02;
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.drawable.dist_sigma = 0.01f;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.type = SURE_DR_MESH;
}

void SureData::SetTemplate_GlowCube(float SideLen){
TemplateObject.ParentID = -1;
    TemplateObject.movable = true;
    TemplateObject.drawable.sided = false;
    TemplateObject.collidable = true;
    TemplateObject.drawable.rgb.s[0] = 80.0;
    TemplateObject.drawable.rgb.s[1] = 250.0;
    TemplateObject.drawable.rgb.s[2] = 60.0;
    TemplateObject.type = SURE_OBJ_MESH;
    TemplateObject.lx = SideLen; // длина
    TemplateObject.ly = SideLen; // ширина
    TemplateObject.lz = SideLen; // высота
    TemplateObject.lp = SideLen*1.7f;
    TemplateObject.drawable.radiance = 1.0f;
    sprintf(TemplateObject.ModelName_drawable,"cube");
    TemplateObject.ModelID_drawable = GetModel(TemplateObject.ModelName_drawable);
    sprintf(TemplateObject.ModelName_collider,"cube");
    TemplateObject.ModelID_collider = GetModel(TemplateObject.ModelName_collider);
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
    TemplateObject.drawable.transp = 0.0;
    TemplateObject.drawable.refr = 9.91f;
    TemplateObject.drawable.transp_i = 0.88;
    TemplateObject.drawable.dist_type = SURE_D_EQUAL;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.dist_sigma = 0.01f;
    TemplateObject.drawable.type = SURE_DR_MESH;
}

void SureData::SetTemplate_RegularCube(){
TemplateObject.ParentID = -1;
    TemplateObject.movable = false;
    TemplateObject.drawable.sided = false;
    TemplateObject.collidable = true;
    TemplateObject.drawable.rgb.s[0] = 100.0;
    TemplateObject.drawable.rgb.s[1] = 100.0;
    TemplateObject.drawable.rgb.s[2] = 100.0;
    TemplateObject.type = SURE_OBJ_MESH;
    TemplateObject.lx = 10.0; // длина
    TemplateObject.ly = 10.0; // ширина
    TemplateObject.lz = 10.0; // высота
    TemplateObject.lp = 17.0;
    TemplateObject.drawable.radiance = 0.0f;
    sprintf(TemplateObject.ModelName_drawable,"cube");
    TemplateObject.ModelID_drawable = GetModel(TemplateObject.ModelName_drawable);
    sprintf(TemplateObject.ModelName_collider,"cube");
    TemplateObject.ModelID_collider = GetModel(TemplateObject.ModelName_collider);
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
    TemplateObject.drawable.transp = 0.0;
    TemplateObject.drawable.refr = 9.91f;
    TemplateObject.drawable.transp_i = 0.88;
    TemplateObject.drawable.dist_type = SURE_D_EQUAL;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.dist_sigma = 0.01f;
    TemplateObject.drawable.type = SURE_DR_MESH;
}

void SureData::SetTemplate_RegularPandus(){
TemplateObject.ParentID = -1;
    TemplateObject.movable = false;
    TemplateObject.collidable = true;
    TemplateObject.drawable.sided = false;
    TemplateObject.drawable.rgb.s[0] = 100.0;
    TemplateObject.drawable.rgb.s[1] = 100.0;
    TemplateObject.drawable.rgb.s[2] = 100.0;
    TemplateObject.type = SURE_OBJ_MESH;
    TemplateObject.lx = 10; // длина
    TemplateObject.ly = 10; // ширина
    TemplateObject.lz = 10; // высота
    TemplateObject.lp = 17;
    TemplateObject.drawable.radiance = 0.0f;
    sprintf(TemplateObject.ModelName_drawable,"pand");
    TemplateObject.ModelID_drawable = GetModel(TemplateObject.ModelName_drawable);
    sprintf(TemplateObject.ModelName_collider,"pand");
    TemplateObject.ModelID_collider = GetModel(TemplateObject.ModelName_collider);
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
    TemplateObject.drawable.transp = 0.0;
    TemplateObject.drawable.refr = 9.91f;
    TemplateObject.drawable.transp_i = 0.88;
    TemplateObject.drawable.dist_type = SURE_D_EQUAL;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.dist_sigma = 0.01f;
    TemplateObject.drawable.type = SURE_DR_MESH;
}

void SureData::SetTemplate_RegularTetr(float SideLen){
TemplateObject.ParentID = -1;
    TemplateObject.movable = true;
    TemplateObject.collidable = true;
    TemplateObject.drawable.sided = false;
    TemplateObject.drawable.rgb.s[0] = 120.0;
    TemplateObject.drawable.rgb.s[1] = 90.0;
    TemplateObject.drawable.rgb.s[2] = 160.0;
    TemplateObject.type = SURE_OBJ_MESH;
    TemplateObject.lx = SideLen; // длина
    TemplateObject.ly = SideLen; // ширина
    TemplateObject.lz = SideLen; // высота
    TemplateObject.lp = SideLen;
    TemplateObject.drawable.radiance = 0.0f;
    sprintf(TemplateObject.ModelName_drawable,"tetr");
    TemplateObject.ModelID_drawable = GetModel(TemplateObject.ModelName_drawable);
    sprintf(TemplateObject.ModelName_collider,"tetr");
    TemplateObject.ModelID_collider = GetModel(TemplateObject.ModelName_collider);
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
    TemplateObject.drawable.transp = 0.0;
    TemplateObject.drawable.refr = 9.91f;
    TemplateObject.drawable.transp_i = 0.88;
    TemplateObject.drawable.dist_type = SURE_D_EQUAL;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.dist_sigma = 0.01f;
    TemplateObject.drawable.type = SURE_DR_MESH;
}

void SureData::SetTemplate_GlowSphere(float Radius){
TemplateObject.ParentID = -1;
    TemplateObject.drawable.rgb.s[0] = 80.0;
    TemplateObject.drawable.rgb.s[1] = 250.0;
    TemplateObject.drawable.rgb.s[2] = 60.0;
    TemplateObject.movable = true;
    TemplateObject.collidable = true;
    TemplateObject.drawable.sided = false;
    TemplateObject.type = SURE_OBJ_SPHERE;
    TemplateObject.drawable.radiance = 1.0;
    TemplateObject.drawable.type = SURE_DR_SPHERE; // форма
    TemplateObject.lx = Radius;
    TemplateObject.lp = Radius*1.7;
    TemplateObject.drawable.transp = 0.0;
    TemplateObject.drawable.refr = 9.21f;
    TemplateObject.drawable.transp_i = 0.88;
    TemplateObject.drawable.dist_type = SURE_D_EQUAL;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.dist_sigma = 0.01f;
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
}

void SureData::SetTemplate_GlowPlane()
{
TemplateObject.ParentID = -1;
    TemplateObject.drawable.rgb.s[0] = 253.0;
    TemplateObject.drawable.rgb.s[1] = 245.0;
    TemplateObject.drawable.rgb.s[2] = 250.0;
    TemplateObject.movable = false;
    TemplateObject.collidable = true;
    TemplateObject.drawable.sided = false;
    TemplateObject.type = SURE_OBJ_PLANE;
    TemplateObject.drawable.radiance = 1.0;
    TemplateObject.drawable.type = SURE_DR_SQUARE; // форма
    TemplateObject.lx = 10.0;
    TemplateObject.ly = 10.0;
    TemplateObject.lz = 10.0;
    TemplateObject.drawable.transp = 0.0;
    TemplateObject.drawable.refr = 9.21f;
    TemplateObject.drawable.transp_i = 0.88;
    TemplateObject.drawable.dist_type = SURE_D_EQUAL;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.dist_sigma = 0.01f;
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
}

void SureData::SetTemplate_RegularPlane()
{
TemplateObject.ParentID = -1;
    TemplateObject.drawable.rgb.s[0] = 100.0;
    TemplateObject.drawable.rgb.s[1] = 100.0;
    TemplateObject.drawable.rgb.s[2] = 100.0;
    TemplateObject.movable = false;
    TemplateObject.collidable = true;
    TemplateObject.drawable.sided = true;
    TemplateObject.type = SURE_OBJ_PLANE;
    TemplateObject.drawable.radiance = 0.0;
    TemplateObject.drawable.type = SURE_DR_SQUARE; // форма
    TemplateObject.lx = 10.0;
    TemplateObject.ly = 10.0;
    TemplateObject.lz = 10.0;
    TemplateObject.drawable.transp = 0.0;
    TemplateObject.drawable.refr = 9.21f;
    TemplateObject.drawable.transp_i = 0.88;
    TemplateObject.drawable.dist_type = SURE_D_EQUAL;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.dist_sigma = 1.01f;
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
}

void SureData::SetTemplate_FogPlane()
{
TemplateObject.ParentID = -1;
    TemplateObject.drawable.rgb.s[0] = 250.0;
    TemplateObject.drawable.rgb.s[1] = 200.0;
    TemplateObject.drawable.rgb.s[2] = 200.0;
    TemplateObject.movable = false;
    TemplateObject.collidable = false;
    TemplateObject.drawable.sided = false;
    TemplateObject.type = SURE_OBJ_PLANE;
    TemplateObject.drawable.radiance = 0.0;
    TemplateObject.drawable.type = SURE_DR_SQUARE; // форма
    TemplateObject.lx = 10.0;
    TemplateObject.ly = 10.0;
    TemplateObject.lz = 10.0;
    TemplateObject.drawable.transp = 1.1;
    TemplateObject.drawable.refr = 1.001f;
    TemplateObject.drawable.transp_i = 0.03;
    TemplateObject.drawable.dist_type = SURE_D_NORM;
    TemplateObject.drawable.dist_m = 0;
    TemplateObject.drawable.dist_sigma = 0.001f;
    TemplateObject.drawable.map_id = -1;
    TemplateObject.drawable.advmap_id = -1;
}
