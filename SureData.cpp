#include "SureData.h"


void SureData::SureClear()
{
    delete TexturesData;
    delete UVMap;
    delete Normals;
    delete VrtxCLImg;
    delete MeshCLImg;
};

void SureData::LoadTexture(const char* name)
{
    QImage img;
    QColor lv_pix;
    char fname[50];
    sprintf(fname,"./maps/%s.png",name);
    if(!img.load(fname))
    {
        std::cout << "Не удалось открыть файл " << fname << "\n";
    }else
    {
        img.convertToFormat(QImage::Format_ARGB32);
        int mx = img.size().width()-1;
        int my = img.size().height()-1;
        cl_uchar* tex = &TexturesData[cur_textures*SURE_R_TEXRES*SURE_R_TEXRES*4];
        for(int iy = 0;iy<SURE_R_TEXRES;++iy)
        for(int ix = 0;ix<SURE_R_TEXRES;++ix)
        {
            float dpx = (float)SURE_R_TEXRES/mx; // количество точек карты на пиксель
            float dpy = (float)SURE_R_TEXRES/my; // количество точек карты на пиксель
            int cx = (float)mx*(float)ix/(float)SURE_R_TEXRES; // текущая x-координата на карте
            int cy = (float)my*(float)iy/(float)SURE_R_TEXRES; // текущая y-координата на карте
            float ddx = (float)ix - (float)cx*dpx; // расстояние (от -0.5 до 0.5 от точной координаты на карте текущего пикселя картинки)
            float ddy = (float)iy - (float)cy*dpy; // расстояние (от -0.5 до 0.5 от точной координаты на карте текущего пикселя картинки)
            ddx /= dpx;
            ddy /= dpy;
            float lv_amountr = 0;
            float lv_amountg = 0;
            float lv_amountb = 0;
            float lv_amounta = 0;
            uchar* img_cursor;
            if(ddx<0){ // корректируем по точке слева
                ddx = -ddx;
                img_cursor = img.scanLine(cy)+(cx <= 0 ? 0 : cx-1)*4;
                lv_amountb += (*img_cursor)*ddx; img_cursor++;
                lv_amountg += (*img_cursor)*ddx; img_cursor++;
                lv_amountr += (*img_cursor)*ddx; img_cursor++;
                lv_amounta += (*img_cursor)*ddx; img_cursor++;
            }else{  // корректируем по точке справа
                img_cursor = img.scanLine(cy)+(cx >= mx ? mx : cx+1)*4;
                lv_amountb += (*img_cursor)*ddx; img_cursor++;
                lv_amountg += (*img_cursor)*ddx; img_cursor++;
                lv_amountr += (*img_cursor)*ddx; img_cursor++;
                lv_amounta += (*img_cursor)*ddx; img_cursor++;
            };
            if(ddy<0){ // корректируем по точке сверху
                ddy = -ddy;
                img_cursor = img.scanLine(cy<=0 ? 0 : cy-1)+(cx)*4;
                lv_amountb += (*img_cursor)*ddy; img_cursor++;
                lv_amountg += (*img_cursor)*ddy; img_cursor++;
                lv_amountr += (*img_cursor)*ddy; img_cursor++;
                lv_amounta += (*img_cursor)*ddy; img_cursor++;
            }else{  // корректируем по точке снизу
                img_cursor = img.scanLine(cy >= my ? my : cy+1)+(cx)*4;
                lv_amountb += (*img_cursor)*ddy; img_cursor++;
                lv_amountg += (*img_cursor)*ddy; img_cursor++;
                lv_amountr += (*img_cursor)*ddy; img_cursor++;
                lv_amounta += (*img_cursor)*ddy; img_cursor++;
            };
            img_cursor = img.scanLine(cy)+(cx)*4;
            lv_amountb += (*img_cursor)*(1-ddx)*(1-ddy); img_cursor++;
            lv_amountg += (*img_cursor)*(1-ddx)*(1-ddy); img_cursor++;
            lv_amountr += (*img_cursor)*(1-ddx)*(1-ddy); img_cursor++;
            lv_amounta += (*img_cursor)*(1-ddx)*(1-ddy); img_cursor++;
            lv_amountr /= (ddx+ddy+(1-ddx)*(1-ddy));
            lv_amountg /= (ddx+ddy+(1-ddx)*(1-ddy));
            lv_amountb /= (ddx+ddy+(1-ddx)*(1-ddy));
            lv_amounta /= (ddx+ddy+(1-ddx)*(1-ddy));
            lv_amountr+=(255.0-lv_amountr)*(255-lv_amounta)/255.0;
            lv_amountg+=(255.0-lv_amountg)*(255-lv_amounta)/255.0;
            lv_amountb+=(255.0-lv_amountb)*(255-lv_amounta)/255.0;
            if(lv_amountr>255)lv_amountr=255;
            if(lv_amountg>255)lv_amountg=255;
            if(lv_amountb>255)lv_amountb=255;
            if(lv_amounta>255)lv_amounta=255;
            *tex = lv_amountb; ++tex;
            *tex = lv_amountg; ++tex;
            *tex = lv_amountr; ++tex;
            *tex = lv_amounta; ++tex;
        };
        TexturesInfo[cur_textures].toupdate = true;
        sprintf(TexturesInfo[cur_textures].name,"%s",name);
        cur_textures++;
    };
};

int SureData::GetTexture(const char* name)
{
    int result = -1;
    for(uint i = 0; (i<cur_textures)&&(result<0);++i)
    if(strlen(name)==strlen(TexturesInfo[i].name))
    {
        result = i;
        for(uint c = 0; c<strlen(name);++c)
        if(TexturesInfo[i].name[c]!=name[c])
            result=-1;
    };
    return result;
};

void SureData::SetDefaultRotationBasis(int i)
{
    objects[i].ox.x = 1;
    objects[i].ox.y = 0;
    objects[i].ox.z = 0;
    objects[i].oy.x = 0;
    objects[i].oy.y = 1;
    objects[i].oy.z = 0;
    objects[i].oz.x = 0;
    objects[i].oz.y = 0;
    objects[i].oz.z = 1;
};

void SureData::ObjCoordsToDrawable(int i)
{
    objects[i].drawable.X  = objects[i].X;
    objects[i].drawable.ox = objects[i].ox;
    objects[i].drawable.oy = objects[i].oy;
    objects[i].drawable.oz = objects[i].oz;
    objects[i].drawable.lx = objects[i].lx;
    objects[i].drawable.ly = objects[i].ly;
    objects[i].drawable.lz = objects[i].lz;
};

int SureData::CreateObject(int type)
{
    int i = m_objects;
    SetDefaultRotationBasis(i);
    objects[i].type = type;
    m_objects++;
    return m_objects - 1;
};

int SureData::AddVertex(double x, double y, double z)
{
    __VERTEX_X(cur_vertexes) = x;
    __VERTEX_Y(cur_vertexes) = y;
    __VERTEX_Z(cur_vertexes) = z;
    ++cur_vertexes;
    return cur_vertexes-1;
};

int SureData::AddVertex(my_double3 X)
{
    __VERTEX_SET(cur_vertexes,X);
    ++cur_vertexes;
    return cur_vertexes-1;
};

int SureData::AddMesh(int v1, int v2, int v3)
{
    __MESH_V1(cur_meshes) = v1;
    __MESH_V2(cur_meshes) = v2;
    __MESH_V3(cur_meshes) = v3;
    __MESH_UV1_U(cur_meshes) = 0;
    __MESH_UV1_V(cur_meshes) = 0;
    __MESH_UV2_U(cur_meshes) = 0;
    __MESH_UV2_V(cur_meshes) = 0;
    __MESH_UV3_U(cur_meshes) = 0;
    __MESH_UV3_V(cur_meshes) = 0;
    ++cur_meshes;
    return cur_meshes-1;
};

void SureData::MapTexture(int object,int type)
{
    double x,y;
    int i = objects[object].mesh_start;
    for(uint mi = 0;mi<objects[object].mesh_count;++mi)
    {
        if(type == SURE_MAPPING_PLANAR_XZ)
        {
            x = objects[object].lx;
            y = objects[object].lz;
            __MESH_UV1_U(i) = __VERTEX_X(__MESH_V1(i))+x;
            __MESH_UV1_V(i) = __VERTEX_Z(__MESH_V1(i))+y;
            __MESH_UV2_U(i) = __VERTEX_X(__MESH_V2(i))+x;
            __MESH_UV2_V(i) = __VERTEX_Z(__MESH_V2(i))+y;
            __MESH_UV3_U(i) = __VERTEX_X(__MESH_V3(i))+x;
            __MESH_UV3_V(i) = __VERTEX_Z(__MESH_V3(i))+y;
        }else if(type == SURE_MAPPING_PLANAR_YZ)
        {
            x = objects[object].ly;
            y = objects[object].lz;
            __MESH_UV1_U(i) = __VERTEX_Y(__MESH_V1(i))+x;
            __MESH_UV1_V(i) = __VERTEX_Z(__MESH_V1(i))+y;
            __MESH_UV2_U(i) = __VERTEX_Y(__MESH_V2(i))+x;
            __MESH_UV2_V(i) = __VERTEX_Z(__MESH_V2(i))+y;
            __MESH_UV3_U(i) = __VERTEX_Y(__MESH_V3(i))+x;
            __MESH_UV3_V(i) = __VERTEX_Z(__MESH_V3(i))+y;
        }else if(type == SURE_MAPPING_SPHERICAL)
        {
            x = 0.5;
            y = 0.5;
            my_double3 v;
            my_double3 vf;
            double mapx1, mapy1;
            double mapx2, mapy2;
            double mapx3, mapy3;

            v.x = __VERTEX_X(__MESH_V1(i));
            v.y = __VERTEX_Y(__MESH_V1(i));
            v.z = __VERTEX_Z(__MESH_V1(i));
            v = __NORMALIZE(v);

            vf.x = 0; vf.y = 0; vf.z = -1;
            mapy1 = 0.5*(dot(v,vf)+1); // от 0 (низ) до 1 (верх)
            if(v.x>0){mapx1 = atan(v.y/v.x)/M_PI+0.5;}
            else{mapx1 = atan(v.y/v.x)/M_PI+1.5;};
            mapx1*=0.5;

            v.x = __VERTEX_X(__MESH_V2(i));
            v.y = __VERTEX_Y(__MESH_V2(i));
            v.z = __VERTEX_Z(__MESH_V2(i));
            v = __NORMALIZE(v);

            vf.x = 0; vf.y = 0; vf.z = -1;
            mapy2 = 0.5*(dot(v,vf)+1); // от 0 (низ) до 1 (верх)
            if(v.x>0){mapx2 = atan(v.y/v.x)/M_PI+0.5;}
            else{mapx2 = atan(v.y/v.x)/M_PI+1.5;};
            mapx2*=0.5;

            v.x = __VERTEX_X(__MESH_V3(i));
            v.y = __VERTEX_Y(__MESH_V3(i));
            v.z = __VERTEX_Z(__MESH_V3(i));
            v = __NORMALIZE(v);

            vf.x = 0; vf.y = 0; vf.z = -1;
            mapy3 = 0.5*(dot(v,vf)+1); // от 0 (низ) до 1 (верх)
            if(v.x>0){mapx3 = atan(v.y/v.x)/M_PI+0.5;}
            else{mapx3 = atan(v.y/v.x)/M_PI+1.5;};
            mapx3*=0.5;

            double maxx = std::max(mapx1,std::max(mapx2,mapx3));
            if((maxx-mapx1)>0.5)mapx1 = mapx1 + 1.0;
            if((maxx-mapx2)>0.5)mapx2 = mapx2 + 1.0;
            if((maxx-mapx3)>0.5)mapx3 = mapx3 + 1.0;

            __MESH_UV1_U(i) = mapx1;
            __MESH_UV1_V(i) = mapy1;
            __MESH_UV2_U(i) = mapx2;
            __MESH_UV2_V(i) = mapy2;
            __MESH_UV3_U(i) = mapx3;
            __MESH_UV3_V(i) = mapy3;

        }else{
            x = objects[object].lx;
            y = objects[object].ly;
            __MESH_UV1_U(i) = __VERTEX_X(__MESH_V1(i))+x;
            __MESH_UV1_V(i) = __VERTEX_Y(__MESH_V1(i))+y;
            __MESH_UV2_U(i) = __VERTEX_X(__MESH_V2(i))+x;
            __MESH_UV2_V(i) = __VERTEX_Y(__MESH_V2(i))+y;
            __MESH_UV3_U(i) = __VERTEX_X(__MESH_V3(i))+x;
            __MESH_UV3_V(i) = __VERTEX_Y(__MESH_V3(i))+y;
        };
        __MESH_UV1_U(i) = 0.5*SURE_R_TEXRES*__MESH_UV1_U(i)/x;
        __MESH_UV1_V(i) = 0.5*SURE_R_TEXRES*__MESH_UV1_V(i)/y;
        __MESH_UV2_U(i) = 0.5*SURE_R_TEXRES*__MESH_UV2_U(i)/x;
        __MESH_UV2_V(i) = 0.5*SURE_R_TEXRES*__MESH_UV2_V(i)/y;
        __MESH_UV3_U(i) = 0.5*SURE_R_TEXRES*__MESH_UV3_U(i)/x;
        __MESH_UV3_V(i) = 0.5*SURE_R_TEXRES*__MESH_UV3_V(i)/y;
        i++;
    };
};

void SureData::Mesh_GenerateCube(int object)
{
    objects[object].mesh_start = cur_meshes;
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
    AddMesh(cv+4,cv+5,cv+6);
    AddMesh(cv+6,cv+7,cv+4);
    AddMesh(cv  ,cv+1,cv+6);
    AddMesh(cv+6,cv+5,cv  );
    AddMesh(cv+1,cv+2,cv+7);
    AddMesh(cv+7,cv+6,cv+1);
    AddMesh(cv+2,cv+3,cv+4);
    AddMesh(cv+4,cv+7,cv+2);
    AddMesh(cv+3,cv  ,cv+5);
    AddMesh(cv+5,cv+4,cv+3);
    objects[object].mesh_count = 12;
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
        AddMesh(AddVertex(vertexes[cover[ic][2]]),
                AddVertex(vertexes[cover[ic][1]]),
                AddVertex(vertexes[cover[ic][0]]));

                if(norm_type==SURE_NORMALS_DEFAULT)
                {
                my_double3 n = __NORMALIZE(cross(vertexes[cover[ic][1]]-vertexes[cover[ic][0]],vertexes[cover[ic][2]]-vertexes[cover[ic][0]]));
                    __NORMAL1_X(objects[object].mesh_count) = n.x;
                    __NORMAL1_Y(objects[object].mesh_count) = n.y;
                    __NORMAL1_Z(objects[object].mesh_count) = n.z;
                    __NORMAL2_X(objects[object].mesh_count) = n.x;
                    __NORMAL2_Y(objects[object].mesh_count) = n.y;
                    __NORMAL2_Z(objects[object].mesh_count) = n.z;
                    __NORMAL3_X(objects[object].mesh_count) = n.x;
                    __NORMAL3_Y(objects[object].mesh_count) = n.y;
                    __NORMAL3_Z(objects[object].mesh_count) = n.z;
                };
                if(norm_type==SURE_NORMALS_SHPERICAL)
                {
                my_double3 n1 = __NORMALIZE(vertexes[cover[ic][2]]);
                my_double3 n2 = __NORMALIZE(vertexes[cover[ic][1]]);
                my_double3 n3 = __NORMALIZE(vertexes[cover[ic][0]]);
                    __NORMAL1_X(objects[object].mesh_count) = n1.x;
                    __NORMAL1_Y(objects[object].mesh_count) = n1.y;
                    __NORMAL1_Z(objects[object].mesh_count) = n1.z;
                    __NORMAL2_X(objects[object].mesh_count) = n2.x;
                    __NORMAL2_Y(objects[object].mesh_count) = n2.y;
                    __NORMAL2_Z(objects[object].mesh_count) = n2.z;
                    __NORMAL3_X(objects[object].mesh_count) = n3.x;
                    __NORMAL3_Y(objects[object].mesh_count) = n3.y;
                    __NORMAL3_Z(objects[object].mesh_count) = n3.z;
                };
        objects[object].mesh_count++;
    };
};

void SureData::Mesh_FromFile(int object,const char* fname)
{
    char filename[100];
    sprintf(filename,"./models/%s.obj",fname);
    FILE *fl = fopen(filename,"r");
    char line[256];

    my_double3 v[10000];
    int vertexes = 1;
    my_double3 vn[10000];
    int normals = 1;
    my_double3 vt[10000];
    int tcoords = 1;
    int f[10000][3][3];
    int faces = 0;
    double x,y,z;

    if(fl!=nullptr)
    {
        std::cout << "Загрузка " << fname << "...\n";
        objects[object].mesh_start = cur_meshes;
        objects[object].mesh_count = 0;
        my_double3 maxcoords;
        my_double3 mincoords;
        maxcoords.x = -SURE_R_MAXDISTANCE;
        maxcoords.y = -SURE_R_MAXDISTANCE;
        maxcoords.z = -SURE_R_MAXDISTANCE;
        mincoords.x = SURE_R_MAXDISTANCE;
        mincoords.y = SURE_R_MAXDISTANCE;
        mincoords.z = SURE_R_MAXDISTANCE;
        while(fscanf(fl, "%s", line)!=EOF)
        {
            if ( strcmp( line, "v" ) == 0 ){
                fscanf(fl,"%lf %lf %lf\n",&x,&y,&z);
                v[vertexes].x = x;
                v[vertexes].y = y;
                v[vertexes].z = z;
                vertexes++;
                if(x>maxcoords.x)maxcoords.x=x;
                if(y>maxcoords.y)maxcoords.y=y;
                if(z>maxcoords.z)maxcoords.z=z;
                if(x<mincoords.x)mincoords.x=x;
                if(y<mincoords.y)mincoords.y=y;
                if(z<mincoords.z)mincoords.z=z;
            }else if ( strcmp( line, "vt" ) == 0 ){
                fscanf(fl,"%lf %lf %lf\n",&x,&y,&z);
                vt[tcoords].x = x;
                vt[tcoords].y = 1.0-y;
                vt[tcoords].z = z;
                tcoords++;
            }else if ( strcmp( line, "vn" ) == 0 ){
                fscanf(fl,"%lf %lf %lf\n",&x,&y,&z);
                vn[normals].x = x;
                vn[normals].y = y;
                vn[normals].z = z;
                vn[normals] = __NORMALIZE(vn[normals]);
                normals++;

            }else if ( strcmp( line, "f" ) == 0 ){
                int v1=-1;
                int v2=-1;
                int v3=-1;
                int v4=-1;
                int vt1=-1;
                int vt2=-1;
                int vt3=-1;
                int vt4=-1;
                int vn1=-1;
                int vn2=-1;
                int vn3=-1;
                int vn4=-1;

                fpos_t pos;
                fgetpos(fl,&pos);

                int lr = 0;

                lr = fscanf(fl,"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",&v1,&vt1,&vn1,&v2,&vt2,&vn2,&v3,&vt3,&vn3,&v4,&vt4,&vn4);
                if(lr!=12)
                {
                fsetpos(fl,&pos); v1=v2=v3=v4=vt1=vt2=vt3=vt4=vn1=vn2=vn3=vn4=-1;
                lr = fscanf(fl,"%d/%d/%d %d/%d/%d %d/%d/%d\n",&v1,&vt1,&vn1,&v2,&vt2,&vn2,&v3,&vt3,&vn3);
                    if(lr!=9)
                    {
                        fsetpos(fl,&pos); v1=v2=v3=v4=vt1=vt2=vt3=vt4=vn1=vn2=vn3=vn4=-1;
                        lr = fscanf(fl,"%d//%d %d//%d %d//%d %d//%d\n",&v1,&vn1,&v2,&vn2,&v3,&vn3,&v4,&vn4);
                        if(lr!=8)
                        {
                            fsetpos(fl,&pos); v1=v2=v3=v4=vt1=vt2=vt3=vt4=vn1=vn2=vn3=vn4=-1;
                            lr = fscanf(fl,"%d//%d %d//%d %d//%d\n",&v1,&vn1,&v2,&vn2,&v3,&vn3);
                            if(lr!=6)
                            {
                                fsetpos(fl,&pos); v1=v2=v3=v4=vt1=vt2=vt3=vt4=vn1=vn2=vn3=vn4=-1;
                                lr = fscanf(fl,"%d %d %d %d\n",&v1,&v2,&v3,&v4);
                                if(lr!=4)
                                {
                                    fsetpos(fl,&pos); v1=v2=v3=v4=vt1=vt2=vt3=vt4=vn1=vn2=vn3=vn4=-1;
                                    lr= fscanf(fl,"%d %d %d\n",&v1,&v2,&v3);
                                    if(lr!=3)
                                    {
                                        fsetpos(fl,&pos); v1=v2=v3=v4=vt1=vt2=vt3=vt4=vn1=vn2=vn3=vn4=-1;
                                        lr = fscanf(fl,"%d/%d %d/%d %d/%d %d/%d\n",&v1,&vt1,&v2,&vt2,&v3,&vt3,&v4,&vt4);
                                        if(lr!=8)
                                        {
                                            fsetpos(fl,&pos); v1=v2=v3=v4=vt1=vt2=vt3=vt4=vn1=vn2=vn3=vn4=-1;
                                            lr = fscanf(fl,"%d/%d %d/%d %d/%d\n",&v1,&vt1,&v2,&vt2,&v3,&vt3);
                                        };
                                    };
                                };
                            };
                        };
                    };
                };

                if(lr>2)
                {
                f[faces][0][0] = v1;
                f[faces][0][1] = vt1;
                f[faces][0][2] = vn1;
                f[faces][1][0] = v2;
                f[faces][1][1] = vt2;
                f[faces][1][2] = vn2;
                f[faces][2][0] = v3;
                f[faces][2][1] = vt3;
                f[faces][2][2] = vn3;
                faces++;
                };

                if(v4>=0)
                {
                f[faces][0][0] = v3;
                f[faces][0][1] = vt3;
                f[faces][0][2] = vn3;
                f[faces][1][0] = v4;
                f[faces][1][1] = vt4;
                f[faces][1][2] = vn4;
                f[faces][2][0] = v1;
                f[faces][2][1] = vt1;
                f[faces][2][2] = vn1;
                faces++;
                };


            };
        };

        std::cout << "vertexes=" << vertexes <<  ";faces=" << faces << "\n";
        /*
        std::cout << "max=" << maxcoords.x << ";" << maxcoords.y << ";" << maxcoords.z << "\n";
        std::cout << "min=" << mincoords.x << ";" << mincoords.y << ";" << mincoords.z << "\n";
        */
        int vstart = cur_vertexes-1;
        objects[object].vertex_start = cur_vertexes;
        objects[object].vertex_count = 0;
        for(int i = 1;i<vertexes;++i)
        {
            v[i] -= (maxcoords+mincoords)*0.5; // сдвигаем в центр
            my_double3 absdist = (maxcoords-mincoords)*0.5;
            double maxdist = __SURE_MAX(__SURE_MAX(absdist.x,absdist.y),absdist.z);
            v[i].x = objects[object].lx*v[i].x/maxdist;
            v[i].y = objects[object].ly*v[i].y/maxdist;
            v[i].z = objects[object].lz*v[i].z/maxdist;
            AddVertex(v[i]);
            objects[object].vertex_count++;
        };
        for(int i = 0;i<faces;++i)
        {
                int m = AddMesh(vstart+f[i][0][0],vstart+f[i][1][0],vstart+f[i][2][0]);
                if(f[i][0][1]>=0)
                {
                    __MESH_UV1_U(m) = vt[f[i][0][1]].x*SURE_R_TEXRES;
                    __MESH_UV1_V(m) = vt[f[i][0][1]].y*SURE_R_TEXRES;
                };
                if(f[i][1][1]>=0)
                {
                    __MESH_UV2_U(m) = vt[f[i][1][1]].x*SURE_R_TEXRES;
                    __MESH_UV2_V(m) = vt[f[i][1][1]].y*SURE_R_TEXRES;
                };
                if(f[i][2][1]>=0)
                {
                    __MESH_UV3_U(m) = vt[f[i][2][1]].x*SURE_R_TEXRES;
                    __MESH_UV3_V(m) = vt[f[i][2][1]].y*SURE_R_TEXRES;
                };
                if(f[i][0][2]>=0) // Нормали заданы
                {
                    __NORMAL1_X(m) = vn[f[i][0][2]].x;
                    __NORMAL1_Y(m) = vn[f[i][0][2]].y;
                    __NORMAL1_Z(m) = vn[f[i][0][2]].z;
                    __NORMAL2_X(m) = vn[f[i][1][2]].x;
                    __NORMAL2_Y(m) = vn[f[i][1][2]].y;
                    __NORMAL2_Z(m) = vn[f[i][1][2]].z;
                    __NORMAL3_X(m) = vn[f[i][2][2]].x;
                    __NORMAL3_Y(m) = vn[f[i][2][2]].y;
                    __NORMAL3_Z(m) = vn[f[i][2][2]].z;
                }else{ // Нормали не заданы
                        my_double3 gm1;
                        my_double3 gm2;
                        my_double3 gm3;
                        __GET_VERTEX(gm1,vstart+f[i][0][0]);
                        __GET_VERTEX(gm2,vstart+f[i][1][0]);
                        __GET_VERTEX(gm3,vstart+f[i][2][0]);
                        my_double3 n = __NORMALIZE(cross(gm2-gm1,gm3-gm1));
                    __NORMAL1_X(m) = n.x;
                    __NORMAL1_Y(m) = n.y;
                    __NORMAL1_Z(m) = n.z;
                    __NORMAL2_X(m) = n.x;
                    __NORMAL2_Y(m) = n.y;
                    __NORMAL2_Z(m) = n.z;
                    __NORMAL3_X(m) = n.x;
                    __NORMAL3_Y(m) = n.y;
                    __NORMAL3_Z(m) = n.z;
                };
                objects[object].mesh_count++;
        };
        /*
        std::cout << "Загрузка завершена." << objects[object].mesh_count <<  "граней начиная с " << objects[object].mesh_start << ". Вот первые 5:\n";
        for(int i = 0;i<5;++i)
        {
            std::cout << "P1(" << VertexData[MeshData[(objects[object].mesh_start+i)*3  ]*3] << ";" << VertexData[MeshData[(objects[object].mesh_start+i)*3  ]*3+1] << ";" << VertexData[MeshData[(objects[object].mesh_start+i)*3  ]*3+2] <<
                      ")  P2(" << VertexData[MeshData[(objects[object].mesh_start+i)*3+1]*3] << ";" << VertexData[MeshData[(objects[object].mesh_start+i)*3+1]*3+1] << ";" << VertexData[MeshData[(objects[object].mesh_start+i)*3+1]*3+2] <<
                      ")  P3(" << VertexData[MeshData[(objects[object].mesh_start+i)*3+2]*3] << ";" << VertexData[MeshData[(objects[object].mesh_start+i)*3+2]*3+1] << ";" << VertexData[MeshData[(objects[object].mesh_start+i)*3+2]*3+2] << ")\n";
        };
        */
    }else{
        std::cout << "Не найдена модель " << fname << "!\n";
    };

};

SureData::SureData()
{

        VrtxCLImg = new cl_float[CLSIZE_VERTEX]; // 256*256
        MeshCLImg = new cl_int[CLSIZE_VERTEX]; //256*256
        TexturesData = new cl_uchar[SURE_R_MAXTEX * SURE_R_TEXRES * SURE_R_TEXRES * 4];
        UVMap = new cl_float[CLSIZE_VERTEX*CLSIZE_MESH_DIM]; // 256*4 x 256
        Normals = new cl_float[CLSIZE_VERTEX*CLSIZE_MESH_DIM]; // 256*4 x 256
        srand(time(NULL));

        LoadTexture("parket");
        LoadTexture("earth_adv");
        LoadTexture("test_alpha");
        LoadTexture("colstones");
        LoadTexture("golem");
        LoadTexture("grid");
        LoadTexture("golem_adv");
        LoadTexture("earth");

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

    // голем
    /*
    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = 0; //Координаты центра
    objects[i].X.y = 0;
    objects[i].X.z = 30;
    objects[i].lx = 30.0; // длина
    objects[i].ly = 30.0; // ширина
    objects[i].lz = 30.0; // высота
    //Mesh_GenerateCube(i);
    Mesh_FromFile(i);
    objects[i].movable = false;
    objects[i].collidable = true;
    objects[i].oz.x = 1;
    objects[i].oz.y = 0;
    objects[i].oz.z = 0;
    objects[i].oy.x = 0;
    objects[i].oy.y = 0;
    objects[i].oy.z = 1;
    objects[i].ox.x = 0;
    objects[i].ox.y = 1;
    objects[i].ox.z = 0;

    ObjCoordsToDrawable(i);
    objects[i].drawable.mesh_start = objects[i].mesh_start;
    objects[i].drawable.mesh_count = objects[i].mesh_count;
    objects[i].drawable.mesh_changed = true;
    objects[i].drawable.map_id = GetTexture("golem");
    objects[i].drawable.type = SURE_DR_MESH; // форма
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.0; // прозрачность
    objects[i].drawable.transp_i = 0.1; // прозрачность
    objects[i].drawable.refr = 1.01; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_EQUAL; // тип рандомизации
    objects[i].drawable.dist_sigma = 0.05; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
    objects[i].drawable.rgb.s[0] = 200.0; // цвет
    objects[i].drawable.rgb.s[1] = 200.0; // цвет
    objects[i].drawable.rgb.s[2] = 255.0; // цвет
    objects[i].drawable.sided = true;
    objects[i].lp = 10;
    objects[i].initp4();
    */

    my_double3 generated_mesh[256];

    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = 0; //Координаты центра
    objects[i].X.y = 0;
    objects[i].X.z = 10;
    objects[i].lx = 20.0; // длина
    objects[i].ly = 20.0; // ширина
    objects[i].lz = 20.0; // высота
    Mesh_FromFile(i,"teapot2");

    /*
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

    Mesh_GenerateHull(i,generated_mesh,200,SURE_NORMALS_SHPERICAL);

     MapTexture(i,SURE_MAPPING_PLANAR_XZ);
    */

    objects[i].movable = false;
    objects[i].collidable = true;
    objects[i].oz.x = 0;
    objects[i].oz.y = 0;
    objects[i].oz.z = 1;
    objects[i].oy.x = 0;
    objects[i].oy.y = 1;
    objects[i].oy.z = 0;
    objects[i].ox.x = 1;
    objects[i].ox.y = 0;
    objects[i].ox.z = 0;

    ObjCoordsToDrawable(i);
    objects[i].drawable.mesh_start = objects[i].mesh_start;
    objects[i].drawable.mesh_count = objects[i].mesh_count;
    objects[i].drawable.mesh_changed = true;
    //objects[i].drawable.map_id = GetTexture("golem");
    //objects[i].drawable.advmap_id = GetTexture("golem_adv");
    objects[i].drawable.type = SURE_DR_MESH; // форма
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.95; // прозрачность
    objects[i].drawable.transp_i = 0.8; // прозрачность
    objects[i].drawable.refr = 1.41; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_NORM; // тип рандомизации
    objects[i].drawable.dist_sigma = 0.05; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
    objects[i].drawable.rgb.s[0] = 200.0; // цвет
    objects[i].drawable.rgb.s[1] = 220.0; // цвет
    objects[i].drawable.rgb.s[2] = 255.0; // цвет
    objects[i].drawable.sided = true;
    objects[i].lp = 10;
    objects[i].initp4();

    //коллайдер падающий
    /*
    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = 20; //Координаты центра
    objects[i].X.y = -20;
    objects[i].X.z = 15;
    objects[i].oz.z = -1;
    objects[i].lx = 30.0; // длина
    objects[i].ly = 20.0; // ширина
    objects[i].lz = 15.0; // высота
    for(int mi = 0;mi<256;++mi)
    {
        generated_mesh[mi].x = (float)rand()/(float)RAND_MAX - 0.5;
        generated_mesh[mi].y = (float)rand()/(float)RAND_MAX - 0.5;
        generated_mesh[mi].z = (float)rand()/(float)RAND_MAX - 0.5;
        generated_mesh[mi].x *= objects[i].lx;
        generated_mesh[mi].y *= objects[i].ly;
        generated_mesh[mi].z *= objects[i].lz;
    };
    generated_mesh[255].x = -1.5*objects[i].lx;
    generated_mesh[255].y = 0;
    generated_mesh[255].z = 0;

    Mesh_GenerateHull(i,generated_mesh,256);
    objects[i].movable = false;
    objects[i].collidable = true;
    ObjCoordsToDrawable(i);
    objects[i].drawable.mesh_start = objects[i].mesh_start;
    objects[i].drawable.mesh_count = objects[i].mesh_count;
    objects[i].drawable.mesh_changed = true;
    objects[i].drawable.type = SURE_DR_MESH; // форма
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.95; // прозрачность
    objects[i].drawable.transp_i = 0.9; // прозрачность
    objects[i].drawable.refr = 1.39; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_NORM; // тип рандомизации
    objects[i].drawable.dist_sigma = 0.02; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
        objects[i].drawable.rgb.s[0] = 200.0; // цвет
        objects[i].drawable.rgb.s[1] = 200.0; // цвет
        objects[i].drawable.rgb.s[2] = 255.0; // цвет
    objects[i].drawable.sided = true;
    objects[i].lp = 13;
    objects[i].initp4();
    */

    //коллайдер MetaMesh
    /*
    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = 25; //Координаты центра
    objects[i].X.y = 25;
    objects[i].X.z = 30;
    objects[i].oz.z = -1;
    objects[i].lx = 25.0; // длина
    objects[i].ly = 25.0; // ширина
    objects[i].lz = 20.0; // высота
    for(int mi = 0;mi<256;++mi)
    {
        generated_mesh[mi].x = (float)rand()/(float)RAND_MAX - 0.5;
        generated_mesh[mi].y = (float)rand()/(float)RAND_MAX - 0.5;
        generated_mesh[mi].z = (float)rand()/(float)RAND_MAX - 0.5;
        generated_mesh[mi].z *= objects[i].lz * (1.0 - fabs(generated_mesh[mi].x) - fabs(generated_mesh[mi].y)) ;
        generated_mesh[mi].x *= 2.0*objects[i].lx;
        generated_mesh[mi].y *= 2.0*objects[i].ly;
    };
    Mesh_GenerateHull(i,generated_mesh,256);
    //Mesh_GenerateCube(i);
    objects[i].movable = false;
    objects[i].collidable = true;
    ObjCoordsToDrawable(i);
    objects[i].drawable.mesh_start = objects[i].mesh_start;
    objects[i].drawable.mesh_count = objects[i].mesh_count;
    objects[i].drawable.mesh_changed = true;
    objects[i].drawable.map_id = GetTexture("colstones");
    MapTexture(i,0);
    objects[i].drawable.type = SURE_DR_MESH; // форма
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.0; // прозрачность
    objects[i].drawable.transp_i = 0.7; // прозрачность
    objects[i].drawable.refr = 1.01; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_NORM; // тип рандомизации
    objects[i].drawable.dist_sigma = 0.01; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
    objects[i].drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.sided = true;
    objects[i].lp = 13;
    objects[i].initp4();


    i = CreateObject(SURE_OBJ_MESH);
    objects[i].X.x = -10; //Координаты центра
    objects[i].X.y = -25;
    objects[i].X.z = 30;
    objects[i].oz.z = -1;
    objects[i].lx = 35.0; // длина
    objects[i].ly = 25.0; // ширина
    objects[i].lz = 20.0; // высота
    for(int mi = 0;mi<256;++mi)
    {
        generated_mesh[mi].x = (float)rand()/(float)RAND_MAX - 0.5;
        generated_mesh[mi].y = (float)rand()/(float)RAND_MAX - 0.5;
        generated_mesh[mi].z = (float)rand()/(float)RAND_MAX - 0.5;
        generated_mesh[mi].z *= 2.0*objects[i].lz;
        generated_mesh[mi].x *= 2.0*objects[i].lx;
        generated_mesh[mi].y *= 2.0*objects[i].ly;
    };
    Mesh_GenerateHull(i,generated_mesh,256);
    //Mesh_GenerateCube(i);
    objects[i].movable = false;
    objects[i].collidable = true;
    ObjCoordsToDrawable(i);
    objects[i].drawable.mesh_start = objects[i].mesh_start;
    objects[i].drawable.mesh_count = objects[i].mesh_count;
    objects[i].drawable.mesh_changed = true;
    objects[i].drawable.map_id = GetTexture("test_alpha");
    MapTexture(i,1);
    objects[i].drawable.type = SURE_DR_MESH; // форма
    objects[i].drawable.radiance = 0.0; // свечение
    objects[i].drawable.transp = 0.0; // прозрачность
    objects[i].drawable.transp_i = 0.7; // прозрачность
    objects[i].drawable.refr = 1.01; // Коэффициент преломления
    objects[i].drawable.dist_type = SURE_D_NORM; // тип рандомизации
    objects[i].drawable.dist_sigma = 0.01; // sigma рандомизации
    objects[i].drawable.dist_m = 0 ; // матожидание рандомизации
    objects[i].drawable.rgb.s[0] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.rgb.s[1] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.rgb.s[2] = 255.0*(float)rand()/(float)RAND_MAX; // цвет
    objects[i].drawable.sided = true;
    objects[i].lp = 13;
    objects[i].initp4();
    */
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

}

SureData::~SureData()
{

}

#include <func_common.c>

void ocl_errtext(cl_int i_ret)
{
    switch(i_ret)
    {
        case  CL_SUCCESS: std::cout << "CL_SUCCESS ";break;
        case  CL_DEVICE_NOT_FOUND: std::cout << "CL_DEVICE_NOT_FOUND ";break;
        case  CL_DEVICE_NOT_AVAILABLE: std::cout << "CL_DEVICE_NOT_AVAILABLE ";break;
        case  CL_COMPILER_NOT_AVAILABLE: std::cout << "CL_COMPILER_NOT_AVAILABLE ";break;
        case  CL_MEM_OBJECT_ALLOCATION_FAILURE: std::cout << "CL_MEM_OBJECT_ALLOCATION_FAILURE ";break;
        case  CL_OUT_OF_RESOURCES: std::cout << "CL_OUT_OF_RESOURCES ";break;
        case  CL_OUT_OF_HOST_MEMORY: std::cout << "CL_OUT_OF_HOST_MEMORY ";break;
        case  CL_PROFILING_INFO_NOT_AVAILABLE: std::cout << "CL_PROFILING_INFO_NOT_AVAILABLE ";break;
        case  CL_MEM_COPY_OVERLAP: std::cout << "CL_MEM_COPY_OVERLAP ";break;
        case  CL_IMAGE_FORMAT_MISMATCH: std::cout << "CL_IMAGE_FORMAT_MISMATCH ";break;
        case  CL_IMAGE_FORMAT_NOT_SUPPORTED: std::cout << "CL_IMAGE_FORMAT_NOT_SUPPORTED ";break;
        case  CL_BUILD_PROGRAM_FAILURE: std::cout << "CL_BUILD_PROGRAM_FAILURE ";break;
        case  CL_MAP_FAILURE: std::cout << "CL_MAP_FAILURE ";break;
        case  CL_MISALIGNED_SUB_BUFFER_OFFSET: std::cout << "CL_MISALIGNED_SUB_BUFFER_OFFSET ";break;
        case  CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: std::cout << "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST ";break;
        case  CL_COMPILE_PROGRAM_FAILURE: std::cout << "CL_COMPILE_PROGRAM_FAILURE ";break;
        case  CL_LINKER_NOT_AVAILABLE: std::cout << "CL_LINKER_NOT_AVAILABLE ";break;
        case  CL_LINK_PROGRAM_FAILURE: std::cout << "CL_LINK_PROGRAM_FAILURE ";break;
        case  CL_DEVICE_PARTITION_FAILED: std::cout << "CL_DEVICE_PARTITION_FAILED ";break;
        case  CL_KERNEL_ARG_INFO_NOT_AVAILABLE: std::cout << "CL_KERNEL_ARG_INFO_NOT_AVAILABLE ";break;
        case  CL_INVALID_VALUE: std::cout << "CL_INVALID_VALUE ";break;
        case  CL_INVALID_DEVICE_TYPE: std::cout << "CL_INVALID_DEVICE_TYPE ";break;
        case  CL_INVALID_PLATFORM: std::cout << "CL_INVALID_PLATFORM ";break;
        case  CL_INVALID_DEVICE: std::cout << "CL_INVALID_DEVICE ";break;
        case  CL_INVALID_CONTEXT: std::cout << "CL_INVALID_CONTEXT ";break;
        case  CL_INVALID_QUEUE_PROPERTIES: std::cout << "CL_INVALID_QUEUE_PROPERTIES ";break;
        case  CL_INVALID_COMMAND_QUEUE: std::cout << "CL_INVALID_COMMAND_QUEUE ";break;
        case  CL_INVALID_HOST_PTR: std::cout << "CL_INVALID_HOST_PTR ";break;
        case  CL_INVALID_MEM_OBJECT: std::cout << "CL_INVALID_MEM_OBJECT ";break;
        case  CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: std::cout << "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR ";break;
        case  CL_INVALID_IMAGE_SIZE: std::cout << "CL_INVALID_IMAGE_SIZE ";break;
        case  CL_INVALID_SAMPLER: std::cout << "CL_INVALID_SAMPLER ";break;
        case  CL_INVALID_BINARY: std::cout << "CL_INVALID_BINARY ";break;
        case  CL_INVALID_BUILD_OPTIONS: std::cout << "CL_INVALID_BUILD_OPTIONS ";break;
        case  CL_INVALID_PROGRAM: std::cout << "CL_INVALID_PROGRAM ";break;
        case  CL_INVALID_PROGRAM_EXECUTABLE: std::cout << "CL_INVALID_PROGRAM_EXECUTABLE ";break;
        case  CL_INVALID_KERNEL_NAME: std::cout << "CL_INVALID_KERNEL_NAME ";break;
        case  CL_INVALID_KERNEL_DEFINITION: std::cout << "CL_INVALID_KERNEL_DEFINITION ";break;
        case  CL_INVALID_KERNEL: std::cout << "CL_INVALID_KERNEL ";break;
        case  CL_INVALID_ARG_INDEX: std::cout << "CL_INVALID_ARG_INDEX ";break;
        case  CL_INVALID_ARG_VALUE: std::cout << "CL_INVALID_ARG_VALUE ";break;
        case  CL_INVALID_ARG_SIZE: std::cout << "CL_INVALID_ARG_SIZE ";break;
        case  CL_INVALID_KERNEL_ARGS: std::cout << "CL_INVALID_KERNEL_ARGS ";break;
        case  CL_INVALID_WORK_DIMENSION: std::cout << "CL_INVALID_WORK_DIMENSION ";break;
        case  CL_INVALID_WORK_GROUP_SIZE: std::cout << "CL_INVALID_WORK_GROUP_SIZE ";break;
        case  CL_INVALID_WORK_ITEM_SIZE: std::cout << "CL_INVALID_WORK_ITEM_SIZE ";break;
        case  CL_INVALID_GLOBAL_OFFSET: std::cout << "CL_INVALID_GLOBAL_OFFSET ";break;
        case  CL_INVALID_EVENT_WAIT_LIST: std::cout << "CL_INVALID_EVENT_WAIT_LIST ";break;
        case  CL_INVALID_EVENT: std::cout << "CL_INVALID_EVENT ";break;
        case  CL_INVALID_OPERATION: std::cout << "CL_INVALID_OPERATION ";break;
        case  CL_INVALID_GL_OBJECT: std::cout << "CL_INVALID_GL_OBJECT ";break;
        case  CL_INVALID_BUFFER_SIZE: std::cout << "CL_INVALID_BUFFER_SIZE ";break;
        case  CL_INVALID_MIP_LEVEL: std::cout << "CL_INVALID_MIP_LEVEL ";break;
        case  CL_INVALID_GLOBAL_WORK_SIZE: std::cout << "CL_INVALID_GLOBAL_WORK_SIZE ";break;
        case  CL_INVALID_PROPERTY: std::cout << "CL_INVALID_PROPERTY ";break;
        case  CL_INVALID_IMAGE_DESCRIPTOR: std::cout << "CL_INVALID_IMAGE_DESCRIPTOR ";break;
        case  CL_INVALID_COMPILER_OPTIONS: std::cout << "CL_INVALID_COMPILER_OPTIONS ";break;
        case  CL_INVALID_LINKER_OPTIONS: std::cout << "CL_INVALID_LINKER_OPTIONS ";break;
        case  CL_INVALID_DEVICE_PARTITION_COUNT: std::cout << "CL_INVALID_DEVICE_PARTITION_COUNT ";break;
    };
};

int SourceFromFile(const char* i_fname,char* e_source)
{
    FILE *fp;
    int e_size = 0;
    fp = fopen(i_fname, "r");
    if (!fp) {
        std::cout << "Не удается найти файл " << i_fname << "\n";
    }else{
        e_size = fread(e_source,1,SURE_CL_MAXSOURCE,fp);
    };
    fclose( fp );
    return e_size;
};

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

my_double3& operator+=(my_double3 &a,const my_double3 &b)
{ a.x+=b.x;a.y+=b.y;a.z+=b.z; return a; }

my_double3& operator-=(my_double3 &a,const my_double3 &b)
{ a.x-=b.x;a.y-=b.y;a.z-=b.z; return a; }

void ObjCollide(SureObject* o1,SureObject* o2,my_double3 pp,my_double3 pd,double pl)
{
    my_double3 v1 = my_double3{0,0,0};
    my_double3 v2 = my_double3{0,0,0};
    if(o1->movable)v1 = o1->velbypp(pp);
    if(o2->movable)v2 = o2->velbypp(pp);
    my_double3 oz = __NORMALIZE(pd);
    double l_v1z = dot(oz,v1);
    double l_v2z = dot(oz,v2);
    my_double3 v1z = oz*l_v1z;
    my_double3 v1xy = v1-v1z;
    my_double3 v2z = oz*l_v2z;
    my_double3 v2xy = v2-v2z;

    if(o2->movable)o2->push(pp,pd,-pl*0.5);
    if(!o1->movable)o2->push(pp,pd,-pl*0.5);
    if(o1->movable)o1->push(pp,pd,pl*0.5);
    if(!o2->movable)o1->push(pp,pd,pl*0.5);

    if(o2->movable)o2->push(pp,v2xy,-1.0);
    if(o1->movable)o1->push(pp,v1xy,-1.0);

 }
