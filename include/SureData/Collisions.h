
struct SurePhysCollision{
    SureObject* Object1;
    SureObject* Object2;
    my_double3 CollisionPoint = {0,0,0};
    my_double3 CollisionVector = {0,0,0};
    double CollisionLength = 0;
};

my_double3 RelativeCoordinatesToGlobal(SureObject *o,my_double3 X);
my_double3 RelativeVectorToGlobal(SureObject *o,my_double3 X);
my_double3 GlobalCoordinatesToRelative(SureObject *o,my_double3 X);
my_double3 GlobalVectorToRelative(SureObject *o,my_double3 X);
void ObjCollide(SureObject* o1,SureObject* o2,my_double3 pp,my_double3 pd,double pl);
bool PhysCollideSphereSphere(SureObject* o1,SureObject* o2,SurePhysCollision *Collision);
bool PhysCollideSpherePlane(SureObject* o1,SureObject* o2,SurePhysCollision *Collision);
bool PhysCollideSphereMesh(SureObject* o1,SureObject* o2,SureData* EngineData,SurePhysCollision *Collision);
bool PhysCollideMeshPlane(SureObject* o1,SureObject* o2,SureData* EngineData,SurePhysCollision *Collision);
bool PhysCollideMeshMesh(SureObject* o1,SureObject* o2,SureData* EngineData,SurePhysCollision *Collision);
bool PhysCollidePSMesh(SureObject* o1,SureObject* o2,SureData* EngineData,SurePhysCollision *Collision);
bool PhysCollidePSSphere(SureObject* o1,SureObject* o2,SurePhysCollision *Collision);
bool PhysCollidePSPlane(SureObject* o1,SureObject* o2,SurePhysCollision *Collision);
bool PhysCollideAABB(SureObject* o1,SureObject* o2);

class SureGJK{
    public:
        SureGJK();
        SureGJK(SureData *i_Engine);
        virtual ~SureGJK();

        SurePhysCollision Collision;

        void SetEngine(SureData *i_Engine);
        void SetupMinkowski(SureObject *o1,SureObject *o2);
        void InitiateLoop();
        bool LoopExit();
        bool TIContains00();
        void LoopNextStep();
        void ExitLoop_Collision();
        void ExpandCoverLoop();
        bool CollisionFound();
        void Clear();

    protected:

    private:
        SureData *EngineData;

        void ExitLoop_NoCollision();
        void InitiateCoverLoop();
        bool CoverExpanded();
        void SetCoverExpanded();
        void CheckCoverIterration();
        void ClearCover();
        void SetCollisionPoint();
        double GetMaximumDistanceTo00();
        bool FindNextTNI();
        bool FindFarestMinkowskiByVector(my_double3 Vector,uint *e_result);
        bool IsCoverFaceVisibleByPoint(uint f,uint point);
        void CheckAndAddFaceToNewCover(uint face,uint fndi);
        void AddFaceToNewCover(uint face);
        void CheckAndAddFaceToNewCover(uint p1, uint p2, uint p3);
        void AddFaceToNewCover(uint p1, uint p2, uint p3);
        void SwitchCover();
        void ExpandCover(uint fndi);
        void SetIncover(uint i);
        void ClearNewCover();
        void SetCollisionByCover();
        my_double3 GetCoverFaceNormal(uint face);
        my_double3 GetCollisionPointByObject(SureObject *o,my_double3 i_vector,double *dist);
        uint GetFarestVertexByObj(SureObject *o,my_double3 i_vector);

        double L1;
        double L2;
        double L3;
        double L4;
        bool exit_no_collision = false;
        bool exit_collision = false;
        bool cover_expanded = false;
        bool collision_found = false;

        my_double3 *M; // разность минкосвского
        bool *incover;
        uint *C;
        uint *C_N;
        uint mc = 0; // количество точек в разности минковского
        uint cc = 0;
        uint cc_n = 0;
        uint iter = 0; // подсчет итерраций - гарантия от зацикливаний
        uint CollisionFace; // номер грани cover'а, ближайшей к 0,0
        uint TI[4] = {0,0,0,0};
        uint TNI[4] = {0,0,0,0};
        void TNI_to_TI();
};
