SureObject::SureObject()
{

}

SureObject::~SureObject()
{
    if((type == SURE_OBJ_PS)&&ParticlesInitiated)
        delete [] Particles;
    ParticlesCounter = 0;
    ParticlesInitiated = false;
}

void SureObject::MemFree()
{
    if((type == SURE_OBJ_PS)&&ParticlesInitiated)
        delete [] Particles;
    ParticlesCounter = 0;
    ParticlesInitiated = false;
}
