
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
        }else if(type == SURE_MAPPING_PLANAR_XY)
        {
            x = objects[object].lx;
            y = objects[object].ly;
            __MESH_UV1_U(i) = __VERTEX_X(__MESH_V1(i))+x;
            __MESH_UV1_V(i) = __VERTEX_Y(__MESH_V1(i))+y;
            __MESH_UV2_U(i) = __VERTEX_X(__MESH_V2(i))+x;
            __MESH_UV2_V(i) = __VERTEX_Y(__MESH_V2(i))+y;
            __MESH_UV3_U(i) = __VERTEX_X(__MESH_V3(i))+x;
            __MESH_UV3_V(i) = __VERTEX_Y(__MESH_V3(i))+y;
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

