
int SureData::LoadModel(const char* name)
{

    char filename[100];

    int result = -1;

    sprintf(filename,"./models/%s.obj",name);
    FILE *fl = fopen(filename,"r");

    my_double3 v[10000];
    int vertexes = 1;
    my_double3 vn[10000];
    int normals = 1;
    my_double3 vt[10000];
    int tcoords = 1;
    int f[10000][3][3];
    int faces = 0;
    sprintf(ModelsInfo[cur_models].name,"%s",name);

    if(fl!=nullptr)
    {
        sprintf(LogLine,"Загрузка %s...",name);
        Log->AddLine(LogLine);
        fwide(fl,1);
        ModelsInfo[cur_models].mesh_start = cur_meshes;
        ModelsInfo[cur_models].mesh_count = 0;
        my_double3 maxcoords;
        my_double3 mincoords;
        maxcoords.x = -SURE_R_MAXDISTANCE;
        maxcoords.y = -SURE_R_MAXDISTANCE;
        maxcoords.z = -SURE_R_MAXDISTANCE;
        mincoords.x = SURE_R_MAXDISTANCE;
        mincoords.y = SURE_R_MAXDISTANCE;
        mincoords.z = SURE_R_MAXDISTANCE;

        wchar_t curr_c;
        wchar_t curr_type;
        curr_type = *L" ";

        int l_group_index;
        int l_vector_index;
        l_group_index = 0;
        l_vector_index = 0;

        int l_fract;
        int l_int;
        //int l_int_mul;
        int l_fract_div;
        int l_num;
        int l_sig;

        bool l_fract_on;
        bool number_found;

        l_fract = 0;
        l_int = 0;
        l_fract_div = 1;
        //l_int_mul = 1;
        l_fract_on = false;
        number_found = false;
        l_num = 0;
        l_sig = 1;

        curr_c = fgetwc(fl);
        while(curr_c!=WEOF)
        {
            if(curr_type==*L" "){
                curr_type = curr_c;
                l_fract = 0;
                l_int = 0;
                l_fract_div = 1;
                l_vector_index = 0;
                //l_int_mul = 1;
                l_sig = 1;
                l_fract_on = false;
                number_found = false;
            };
            if((curr_type==*L"v")&&(curr_c==*L"t")){
                curr_type=*L"t";
            };
            if((curr_type==*L"v")&&(curr_c==*L"n")){
                curr_type=*L"n";
            };
            if((curr_c==*L" ")||(curr_c==*L"\n")){
                if(curr_type==*L"f"){
                    f[faces][l_group_index][l_vector_index] = l_int;
                };
                if(l_group_index==0){
                    if(curr_type==*L"v"){
                        v[vertexes].x = ((double) l_int + (double) l_fract / (double)l_fract_div)*(double)l_sig;
                    };
                    if(curr_type==*L"n"){
                        vn[normals].x = ((double) l_int + (double) l_fract / (double)l_fract_div)*(double)l_sig;
                    };
                    if(curr_type==*L"t"){
                        vt[tcoords].x = ((double) l_int + (double) l_fract / (double)l_fract_div)*(double)l_sig;
                    };
                };
                if(l_group_index==1){
                    if(curr_type==*L"v"){
                        v[vertexes].y = ((double) l_int + (double) l_fract / (double)l_fract_div)*(double)l_sig;
                    };
                    if(curr_type==*L"n"){
                        vn[normals].y = ((double) l_int + (double) l_fract / (double)l_fract_div)*(double)l_sig;
                    };
                    if(curr_type==*L"t"){
                        vt[tcoords].y = 1.0 - ((double) l_int + (double) l_fract / (double)l_fract_div)*(double)l_sig;
                    };
                };
                if(l_group_index==2){
                    if(curr_type==*L"v"){
                        v[vertexes].z = ((double) l_int + (double) l_fract / (double)l_fract_div)*(double)l_sig;
                    };
                    if(curr_type==*L"n"){
                        vn[normals].z = ((double) l_int + (double) l_fract / (double)l_fract_div)*(double)l_sig;
                    };
                    if(curr_type==*L"t"){
                        vt[tcoords].z = ((double) l_int + (double) l_fract / (double)l_fract_div)*(double)l_sig;
                    };
                };
                if(number_found)
                    ++l_group_index;
                if((l_group_index==3)&&(curr_type==*L"f")&&(curr_c==*L" ")){
                    faces++;
                    f[faces][0][0] = f[faces-1][2][0];
                    f[faces][0][1] = f[faces-1][2][1];
                    f[faces][0][2] = f[faces-1][2][2];
                    f[faces][1][0] = 0;
                    f[faces][1][1] = 0;
                    f[faces][1][2] = 0;
                    f[faces][2][0] = f[faces-1][0][0];
                    f[faces][2][1] = f[faces-1][0][1];
                    f[faces][2][2] = f[faces-1][0][2];
                    l_group_index = 1;
                };
                l_fract = 0;
                l_int = 0;
                l_fract_div = 1;
                l_sig = 1;
                //l_int_mul = 1;
                l_vector_index = 0;
                l_fract_on = false;
                number_found = false;
            };
            if(curr_c==*L"\n"){
                if(curr_type==*L"v"){
                    if(v[vertexes].x>maxcoords.x)maxcoords.x=v[vertexes].x;
                    if(v[vertexes].y>maxcoords.y)maxcoords.y=v[vertexes].y;
                    if(v[vertexes].z>maxcoords.z)maxcoords.z=v[vertexes].z;
                    if(v[vertexes].x<mincoords.x)mincoords.x=v[vertexes].x;
                    if(v[vertexes].y<mincoords.y)mincoords.y=v[vertexes].y;
                    if(v[vertexes].z<mincoords.z)mincoords.z=v[vertexes].z;
                    vertexes++;
                };
                if(curr_type==*L"n"){
                    vn[normals] = __NORMALIZE(vn[normals]);
                    normals++;
                };
                if(curr_type==*L"t"){
                    tcoords++;
                };
                if(curr_type==*L"f"){
                    if((f[faces][1][0]!=0)||(f[faces][1][1]!=0)||(f[faces][1][2]!=0))
                        faces++;
                };
                curr_type = *L" ";
                l_group_index = 0;
                l_vector_index = 0;
                l_fract = 0;
                l_int = 0;
                l_fract_div = 1;
                //l_int_mul = 1;
                l_fract_on = false;
                number_found = false;
                l_sig = 1;
            };
            if(curr_c==*L"/"){
                if(curr_type==*L"f"){
                    f[faces][l_group_index][l_vector_index] = l_int;
                };
                l_fract = 0;
                l_int = 0;
                l_fract_div = 1;
                number_found = false;
                //l_int_mul = 1;
                l_sig = 1;
                ++l_vector_index;
                l_fract_on = false;
            };
            if((curr_c==*L".")||(curr_c==*L",")){
                l_fract_on = true;
            };
            if(curr_c==*L"-"){
                l_sig = -1;
            };
            if((curr_c==*L"1")||(curr_c==*L"2")||(curr_c==*L"3")||(curr_c==*L"4")||
               (curr_c==*L"5")||(curr_c==*L"6")||(curr_c==*L"7")||(curr_c==*L"8")||
               (curr_c==*L"9")||(curr_c==*L"0")){
               number_found = true;
                if(curr_c==*L"1")l_num=1;
                if(curr_c==*L"2")l_num=2;
                if(curr_c==*L"3")l_num=3;
                if(curr_c==*L"4")l_num=4;
                if(curr_c==*L"5")l_num=5;
                if(curr_c==*L"6")l_num=6;
                if(curr_c==*L"7")l_num=7;
                if(curr_c==*L"8")l_num=8;
                if(curr_c==*L"9")l_num=9;
                if(curr_c==*L"0")l_num=0;
                if(l_fract_on){
                    l_fract = 10*l_fract + l_num;
                    l_fract_div *= 10;
                }else{
                    l_int = 10*l_int + l_num;
                };
            };
            curr_c = fgetwc(fl);
        };

        sprintf(LogLine,"%i вершин %i граней загружено",vertexes,faces);
        Log->AddLine(LogLine);

        int vstart = cur_vertexes-1;
        ModelsInfo[cur_models].vertex_start = cur_vertexes;
        ModelsInfo[cur_models].vertex_count = 0;
        for(int i = 1;i<vertexes;++i)
        {
            v[i] -= (maxcoords+mincoords)*0.5; // сдвигаем в центр
            my_double3 absdist = (maxcoords-mincoords)*0.5f;
            double maxdist = __SURE_MAX(__SURE_MAX(absdist.x,absdist.y),absdist.z);
            v[i].x = v[i].x/maxdist;
            v[i].y = v[i].y/maxdist;
            v[i].z = v[i].z/maxdist;
            AddVertex(v[i]);
            ModelsInfo[cur_models].vertex_count++;
        };
        for(int i = 0;i<faces;++i)
        {
                int m = AddMesh(vstart+f[i][0][0],vstart+f[i][1][0],vstart+f[i][2][0]);
                if(f[i][0][1]>=0)
                {
                    __MESH_UV1_U(m) = vt[f[i][0][1]].x*(float)SURE_R_TEXRES;
                    __MESH_UV1_V(m) = vt[f[i][0][1]].y*(float)SURE_R_TEXRES;
                };
                if(f[i][1][1]>=0)
                {
                    __MESH_UV2_U(m) = vt[f[i][1][1]].x*(float)SURE_R_TEXRES;
                    __MESH_UV2_V(m) = vt[f[i][1][1]].y*(float)SURE_R_TEXRES;
                };
                if(f[i][2][1]>=0)
                {
                    __MESH_UV3_U(m) = vt[f[i][2][1]].x*(float)SURE_R_TEXRES;
                    __MESH_UV3_V(m) = vt[f[i][2][1]].y*(float)SURE_R_TEXRES;
                };
                if((f[i][0][2]+f[i][1][2]+f[i][2][2])>0) // Нормали заданы
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
                ModelsInfo[cur_models].mesh_count++;
        };
    }else{
        sprintf(LogLine,"Не найдена модель %s",name);
        Log->AddLine(LogLine);
    };

    sprintf(ModelsInfo[cur_models].name,"%s",name);
    ModelsInfo[cur_models].toupdate = true;
    result = cur_models;
    cur_models++;
    return result;
}

int SureData::LoadTexture(const char* name)
{
    QImage img;
    QColor lv_pix;
    int result = -1;
    char fname[50];
    sprintf(fname,"./maps/%s.png",name);
    if(!img.load(fname))
    {
        sprintf(LogLine,"Не удалось открыть файл %s",fname);
        Log->AddLine(LogLine);
    }else
    {
        img = img.convertToFormat(QImage::Format_ARGB32);
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
        result = cur_textures;
        cur_textures++;
    };
    return result;
}

int SureData::GetModel(const char* name)
{
    int result = -1;
    for(uint i = 0; (i<cur_models)&&(result<0);++i)
    if(strlen(name)==strlen(ModelsInfo[i].name))
    {
        result = i;
        for(uint c = 0; c<strlen(name);++c)
        if(ModelsInfo[i].name[c]!=name[c])
            result=-1;
    };
    if(result<0)
        result = LoadModel(name);
    return result;
}

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
    if(result<0)
        result = LoadTexture(name);
    return result;
}

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
}

void SureData::ObjCoordsToDrawable(int i)
{
    objects[i].drawable.X  = objects[i].X;
    objects[i].drawable.ox = objects[i].ox;
    objects[i].drawable.oy = objects[i].oy;
    objects[i].drawable.oz = objects[i].oz;
    objects[i].drawable.lx = objects[i].lx;
    objects[i].drawable.ly = objects[i].ly;
    objects[i].drawable.lz = objects[i].lz;
}

void SureData::DeleteObjectByID(int i_ID)
{
    uint ObjectsToDelete[50];
    uint ObjectsToDeleteCounter = 0;

    for(uint i = 0;i<(uint)m_objects;++i)
    if((objects[i].ParentID==i_ID)||(objects[i].external_id==i_ID))
        ObjectsToDelete[ObjectsToDeleteCounter++]=i;

    for(int i = ObjectsToDeleteCounter - 1;i>=0;--i)
        DeleteObject(ObjectsToDelete[i]);
}

void SureData::DeleteObject(uint i_object)
{
    if(i_object>=(uint)m_objects)return;
    int LocalExtID = objects[i_object].external_id;
    objects[i_object].MemFree();
    m_objects--;
    objects[i_object] = objects[m_objects];
    for(int il=m_links-1;il>=0;--il)
    if((links[il].Object1==LocalExtID)||(links[il].Object2==LocalExtID)){
        DeleteLink(il);
    };
}

void SureData::DeleteLink(uint i_link){
    if(i_link>=(uint)m_links)return;
    m_links--;
    links[i_link] = links[m_links];
}

uint SureData::CreateObjectFromTemplate(__VTYPE3* i_X)
{
    int i = m_objects;
    m_objects++;

    SetDefaultRotationBasis(i);
    objects[i].type = TemplateObject.type;
    objects[i].mass = TemplateObject.mass;
    objects[i].X = *i_X;
    objects[i].ox = TemplateObject.ox;
    objects[i].oy = TemplateObject.oy;
    objects[i].oz = TemplateObject.oz;
    objects[i].lx = TemplateObject.lx;
    objects[i].ly = TemplateObject.ly;
    objects[i].lz = TemplateObject.lz;
    objects[i].lp = TemplateObject.lp;
    objects[i].X_byparent = TemplateObject.X_byparent;
    objects[i].ox_byparent = TemplateObject.ox_byparent;
    objects[i].oy_byparent = TemplateObject.oy_byparent;
    objects[i].movable = TemplateObject.movable;
    objects[i].collidable = TemplateObject.collidable;
    objects[i].CollideExcludeID = TemplateObject.CollideExcludeID;
   // objects[i].drawable = TemplateObject.drawable;

        objects[i].drawable.X  = objects[i].X;
        objects[i].drawable.ox = objects[i].ox;
        objects[i].drawable.oy = objects[i].oy;
        objects[i].drawable.oz = objects[i].oz;
        objects[i].drawable.lx = objects[i].lx;
        objects[i].drawable.ly = objects[i].ly;
        objects[i].drawable.lz = objects[i].lz;
        objects[i].drawable.type = TemplateObject.drawable.type;
        objects[i].drawable.radiance = TemplateObject.drawable.radiance;
        objects[i].drawable.transp = TemplateObject.drawable.transp;
        objects[i].drawable.transp_i = TemplateObject.drawable.transp_i;
        objects[i].drawable.refr = TemplateObject.drawable.refr;
        objects[i].drawable.dist_type = TemplateObject.drawable.dist_type;
        objects[i].drawable.dist_sigma = TemplateObject.drawable.dist_sigma;
        objects[i].drawable.dist_m = TemplateObject.drawable.dist_m;
        objects[i].drawable.rgb = TemplateObject.drawable.rgb;
        objects[i].drawable.sided = TemplateObject.drawable.sided;
        objects[i].drawable.map_id = TemplateObject.drawable.map_id;
        objects[i].drawable.advmap_id = TemplateObject.drawable.advmap_id;
        objects[i].ModelID_collider = TemplateObject.ModelID_collider;
        sprintf(objects[i].ModelName_collider,"%s",TemplateObject.ModelName_collider);
        objects[i].ModelID_drawable = TemplateObject.ModelID_drawable;
        sprintf(objects[i].ModelName_drawable,"%s",TemplateObject.ModelName_drawable);

    if(TemplateObject.type==SURE_OBJ_MESH)
    {
        // передать vertex'ы и mesh'ы с нормалями
        objects[i].drawable.mesh_start = TemplateObject.drawable.mesh_start;
        objects[i].drawable.mesh_count = TemplateObject.drawable.mesh_count;
    };

    objects[i].ParentID = TemplateObject.ParentID;
    if(objects[i].ParentID<0){
        if(objects[i].movable){
            objects[i].initp4();
            objects[i].movebyp4();
        };
    };
    if(objects[i].ParentID>=0)
        objects[i].align_byparent(ObjByID(objects[i].ParentID));
    objects[i].external_id = m_ObjExternID++;
    return objects[i].external_id;
}

SureObject *SureData::ObjByID(int i_id)
{
    SureObject *result;
    result = &TemplateObject;
    for(int i = 0;i<m_objects;++i)
        if(objects[i].external_id==i_id)
            return &objects[i];
    return result;
}

uint SureData::ObjectNumberByID(int i_id)
{
    for(int i = 0;i<m_objects;++i)
        if(objects[i].external_id==i_id)
            return i;
    return m_objects;
}

int SureData::AddVertex(double x, double y, double z)
{
    __VERTEX_X(cur_vertexes) = (float) x;
    __VERTEX_Y(cur_vertexes) = (float) y;
    __VERTEX_Z(cur_vertexes) = (float) z;
    ++cur_vertexes;
    return cur_vertexes-1;
}

int SureData::AddVertex(my_double3 X)
{
    __VERTEX_SET(cur_vertexes,X);
    ++cur_vertexes;
    return cur_vertexes-1;
}

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
}

void SureData::MapTexture(int i_model,int type)
{
    double x,y;
    int i = ModelsInfo[i_model].mesh_start;
    for(uint mi = 0;mi<ModelsInfo[i_model].mesh_count;++mi)
    {
        x = 1.0;
        y = 1.0;
        if(type == SURE_MAPPING_PLANAR_XZ)
        {
            __MESH_UV1_U(i) = __VERTEX_X(__MESH_V1(i))+x;
            __MESH_UV1_V(i) = __VERTEX_Z(__MESH_V1(i))+y;
            __MESH_UV2_U(i) = __VERTEX_X(__MESH_V2(i))+x;
            __MESH_UV2_V(i) = __VERTEX_Z(__MESH_V2(i))+y;
            __MESH_UV3_U(i) = __VERTEX_X(__MESH_V3(i))+x;
            __MESH_UV3_V(i) = __VERTEX_Z(__MESH_V3(i))+y;
        }else if(type == SURE_MAPPING_PLANAR_YZ)
        {
            __MESH_UV1_U(i) = __VERTEX_Y(__MESH_V1(i))+x;
            __MESH_UV1_V(i) = __VERTEX_Z(__MESH_V1(i))+y;
            __MESH_UV2_U(i) = __VERTEX_Y(__MESH_V2(i))+x;
            __MESH_UV2_V(i) = __VERTEX_Z(__MESH_V2(i))+y;
            __MESH_UV3_U(i) = __VERTEX_Y(__MESH_V3(i))+x;
            __MESH_UV3_V(i) = __VERTEX_Z(__MESH_V3(i))+y;
        }else if(type == SURE_MAPPING_PLANAR_XY)
        {
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
}

