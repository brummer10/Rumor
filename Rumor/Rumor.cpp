
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <cstring>
#include <unistd.h>

#include <lv2.h>

#include "resampler.cc"
#include "resampler-table.cc"
#include "zita-resampler/resampler.h"
#include "gx_resampler.cc"
#include "rumor.cc"
///////////////////////// MACRO SUPPORT ////////////////////////////////

#define PLUGIN_URI "https://github.com/brummer10/Rumor"

#ifndef max
#define max(x, y) (((x) > (y)) ? (x) : (y))
#endif
#ifndef min
#define min(x, y) (((x) < (y)) ? (x) : (y))
#endif


////////////////////////////// PLUG-IN CLASS ///////////////////////////

namespace rumor {

class Xrumor
{
private:
    float* input0;
    float* output0;
    float* bypass;
    float bypass_;
    float* intensity;
    float intensity_;
    float* volume;
    float volume_;
    rumor::Dsp* dsp;
    // bypass ramping
    bool needs_ramp_down;
    bool needs_ramp_up;
    float ramp_down;
    float ramp_up;
    float ramp_up_step;
    float ramp_down_step;
    bool bypassed;

    // private functions
    inline void run_dsp_(uint32_t n_samples);
    inline void connect_(uint32_t port,void* data);
    inline void init_dsp_(uint32_t rate);
    inline void connect_all__ports(uint32_t port, void* data);
    inline void activate_f();
    inline void clean_up();
    inline void deactivate_f();
public:
    // LV2 Descriptor
    static const LV2_Descriptor descriptor;
    // static wrapper to private functions
    static void deactivate(LV2_Handle instance);
    static void cleanup(LV2_Handle instance);
    static void run(LV2_Handle instance, uint32_t n_samples);
    static void activate(LV2_Handle instance);
    static void connect_port(LV2_Handle instance, uint32_t port, void* data);
    static LV2_Handle instantiate(const LV2_Descriptor* descriptor,
                                double rate, const char* bundle_path,
                                const LV2_Feature* const* features);
    Xrumor();
    ~Xrumor();
};

// constructor
Xrumor::Xrumor() :

    input0(NULL),
    output0(NULL),
    bypass(NULL),
    bypass_(2),
    intensity(NULL),
    volume(NULL),
    dsp(rumor::plugin()),
    needs_ramp_down(false),
    needs_ramp_up(false),
    bypassed(false) {};

// destructor
Xrumor::~Xrumor() {
    dsp->del_instance(dsp);
};

///////////////////////// PRIVATE CLASS  FUNCTIONS /////////////////////

void Xrumor::init_dsp_(uint32_t rate)
{
    dsp->init(rate);
    // set values for internal ramping
    ramp_down_step = 32 * (256 * rate) / 48000; 
    ramp_up_step = ramp_down_step;
    ramp_down = ramp_down_step;
    ramp_up = 0.0;
}

// connect the Ports used by the plug-in class
void Xrumor::connect_(uint32_t port,void* data)
{
    switch ((PortIndex)port)
    {
        case 0:
            input0 = static_cast<float*>(data);
            break;
        case 1:
            output0 = static_cast<float*>(data);
            break;
        case 2:
            bypass = static_cast<float*>(data);
            break;
        case 3:
            intensity = static_cast<float*>(data);
            break;
        case 4:
            volume = static_cast<float*>(data);
            break;
        default:
            break;
    }
}

void Xrumor::activate_f()
{
    // allocate the internal DSP mem
}

void Xrumor::clean_up()
{
    // delete the internal DSP mem
}

void Xrumor::deactivate_f()
{
    // delete the internal DSP mem
}

void Xrumor::run_dsp_(uint32_t n_samples)
{
    if(n_samples<1) return;

    // get controller values
#define  intensity_ (*(intensity))
#define  volume_ (*(volume))

    // do inplace processing on default
    if(output0 != input0)
        memcpy(output0, input0, n_samples*sizeof(float));

    float buf0[n_samples];
    // check if bypass is pressed
    if (bypass_ != static_cast<uint32_t>(*(bypass))) {
        bypass_ = static_cast<uint32_t>(*(bypass));
        if (!bypass_) {
            needs_ramp_down = true;
            needs_ramp_up = false;
        } else {
            needs_ramp_down = false;
            needs_ramp_up = true;
            bypassed = false;
        }
    }

    if (needs_ramp_down || needs_ramp_up) {
         memcpy(buf0, input0, n_samples*sizeof(float));
    }
    if (!bypassed) {
        dsp->compute(n_samples, output0, output0);
    }
    // check if ramping is needed
    if (needs_ramp_down) {
        float fade = 0;
        for (uint32_t i=0; i<n_samples; i++) {
            if (ramp_down >= 0.0) {
                --ramp_down; 
            }
            fade = max(0.0,ramp_down) /ramp_down_step ;
            output0[i] = output0[i] * fade + buf0[i] * (1.0 - fade);
        }
        if (ramp_down <= 0.0) {
            // when ramped down, clear buffer from dsp
            needs_ramp_down = false;
            bypassed = true;
            ramp_down = ramp_down_step;
            ramp_up = 0.0;
        } else {
            ramp_up = ramp_down;
        }
    } else if (needs_ramp_up) {
        float fade = 0;
        for (uint32_t i=0; i<n_samples; i++) {
            if (ramp_up < ramp_up_step) {
                ++ramp_up ;
            }
            fade = min(ramp_up_step,ramp_up) /ramp_up_step ;
            output0[i] = output0[i] * fade + buf0[i] * (1.0 - fade);
        }
        if (ramp_up >= ramp_up_step) {
            needs_ramp_up = false;
            ramp_up = 0.0;
            ramp_down = ramp_down_step;
        } else {
            ramp_down = ramp_up;
        }
    }
#undef  intensity_
#undef  volume_
}

void Xrumor::connect_all__ports(uint32_t port, void* data)
{
    // connect the Ports used by the plug-in class
    connect_(port,data);
    dsp->connect(port,data);
}

////////////////////// STATIC CLASS  FUNCTIONS  ////////////////////////

LV2_Handle 
Xrumor::instantiate(const LV2_Descriptor* descriptor,
                            double rate, const char* bundle_path,
                            const LV2_Feature* const* features)
{
    // init the plug-in class
    Xrumor *self = new Xrumor();
    if (!self) {
        return NULL;
    }
    self->init_dsp_((uint32_t)rate);
    return (LV2_Handle)self;
}

void Xrumor::connect_port(LV2_Handle instance, 
                                   uint32_t port, void* data)
{
    // connect all ports
    static_cast<Xrumor*>(instance)->connect_all__ports(port, data);
}

void Xrumor::activate(LV2_Handle instance)
{
    // allocate needed mem
    static_cast<Xrumor*>(instance)->activate_f();
}

void Xrumor::run(LV2_Handle instance, uint32_t n_samples)
{
    // run dsp
    static_cast<Xrumor*>(instance)->run_dsp_(n_samples);
}

void Xrumor::deactivate(LV2_Handle instance)
{
    // free allocated mem
    static_cast<Xrumor*>(instance)->deactivate_f();
}

void Xrumor::cleanup(LV2_Handle instance)
{
    // well, clean up after us
    Xrumor* self = static_cast<Xrumor*>(instance);
    self->clean_up();
    delete self;
}

const LV2_Descriptor Xrumor::descriptor =
{
    PLUGIN_URI ,
    Xrumor::instantiate,
    Xrumor::connect_port,
    Xrumor::activate,
    Xrumor::run,
    Xrumor::deactivate,
    Xrumor::cleanup,
    NULL
};

} // end namespace rumor

////////////////////////// LV2 SYMBOL EXPORT ///////////////////////////

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
    switch (index)
    {
        case 0:
            return &rumor::Xrumor::descriptor;
        default:
            return NULL;
    }
}

///////////////////////////// FIN //////////////////////////////////////