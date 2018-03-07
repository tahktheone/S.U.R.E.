void SureData::Mesh_GenerateTetr(int object,int norm_type)
{
    objects[object].mesh_start = cur_meshes;
    objects[object].vertex_start = cur_vertexes;
    my_double3 p1,p2,p3,p4;

    p1.x = SURE_P4_X*objects[object].lx;
    p1.y = 0;
    p1.z = -SURE_P4_Y*objects[object].lz;
    p2.x = -SURE_P4_Y*objects[object].lx;
    p2.y = 0.5*objects[object].ly;
    p2.z = -SURE_P4_Y*objects[object].lz;
    p3.x = -SURE_P4_Y*objects[object].lx;
    p3.y = -0.5*objects[object].ly;
    p3.z = -SURE_P4_Y*objects[object].lz;
    p4.x = 0;
    p4.y = 0;
    p4.z = SURE_P4_Z*objects[object].lz;

    int cv = AddVertex(p1); // cv
    AddVertex(p2);
    AddVertex(p3);
    AddVertex(p4);

    AddMesh(cv  ,cv+2,cv+1); // нижняя грань
    AddMesh(cv  ,cv+3,cv+2); // +x +y
    AddMesh(cv  ,cv+1,cv+3); // +x -y
    AddMesh(cv+1,cv+2,cv+3); // -x
    objects[object].mesh_count = 4;
    objects[object].vertex_count = cur_vertexes - objects[object].vertex_start;

    Mesh_GenNormals(object,norm_type);

};

void SureData::Mesh_GenNormals(int object, int norm_type)
{
for(int ms = 0; ms<objects[object].mesh_count; ++ms)
    {
        int im = objects[object].mesh_start + ms;
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

};

void SureData::Mesh_GenerateCube(int object,int norm_type)
{
    objects[object].mesh_start = cur_meshes;
    objects[object].vertex_start = cur_vertexes;
    double x = objects[object].lx;
    double y = objects[object].ly;
    double z = objects[object].lz;
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
    objects[object].mesh_count = 12;
    objects[object].vertex_count = cur_vertexes - objects[object].vertex_start;

    Mesh_GenNormals(object,norm_type);

};

void SureData::Mesh_GenerateHull(int object,my_double3* vertexes,int vert_count,int norm_type)
{
    objects[object].mesh_start = cur_meshes;

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

    objects[object].mesh_count = 0;
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
        objects[object].mesh_count++;
    };
};


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

};