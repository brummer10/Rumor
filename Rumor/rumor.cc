// generated from file 'rumor.dsp' by dsp2cc:
// Code generated with Faust 2.37.3 (https://faust.grame.fr)

#include <cmath>
#include <algorithm>

#define FAUSTFLOAT float

using std::signbit;

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

template<class T> inline T mydsp_faustpower2_f(T x) {return (x * x);}
template<class T> inline T mydsp_faustpower3_f(T x) {return ((x * x) * x);}
template<class T> inline T mydsp_faustpower4_f(T x) {return (((x * x) * x) * x);}
template<class T> inline T mydsp_faustpower5_f(T x) {return ((((x * x) * x) * x) * x);}
template<class T> inline T mydsp_faustpower6_f(T x) {return (((((x * x) * x) * x) * x) * x);}

#define always_inline inline __attribute__((__always_inline__))

typedef enum
{
   input0,
   output0,
   bypass,
   INTENSITY, 
   LEVEL, 
} PortIndex;

#include "rumor_table.h"

namespace rumor {

class Dsp {
private:
#ifndef  __MOD_DEVICES__
	gx_resample::FixedRateResampler smp;
	uint32_t sample_rate;
#endif
	uint32_t fSampleRate;
	float fRec7[2];
	float fRec5[2];
	float fRec3[2];
	float fRec1[2];
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	FAUSTFLOAT fVslider0;
	FAUSTFLOAT	*fVslider0_;
	float fRec10[2];
	float fConst5;
	float fConst7;
	float fConst8;
	float fConst9;
	float fRec9[3];
	float fRec0[2];
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT	*fHslider0_;
	float fRec11[2];

#ifndef  __MOD_DEVICES__
	double lowpass_fVec0[2];
	double lowpass_fRec1[2];
	double lowpass_fRec0[3];
#endif
public:
	void connect(uint32_t port,void* data);
	void del_instance(Dsp *p);
	void clear_state_f();
	void init(uint32_t sample_rate);
	void compute(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0);
	Dsp();
	~Dsp();
};



Dsp::Dsp() {
}

Dsp::~Dsp() {
}

inline void Dsp::clear_state_f()
{
	for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) fRec7[l0] = 0.0f;
	for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) fRec5[l1] = 0.0f;
	for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) fRec3[l2] = 0.0f;
	for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) fRec1[l3] = 0.0f;
	for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) fRec10[l4] = 0.0f;
	for (int l5 = 0; (l5 < 3); l5 = (l5 + 1)) fRec9[l5] = 0.0f;
	for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) fRec0[l6] = 0.0f;
	for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) fRec11[l7] = 0.0f;
#ifndef  __MOD_DEVICES__
	for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) lowpass_fVec0[l0] = 0.0;
	for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) lowpass_fRec1[l1] = 0.0;
	for (int l2 = 0; (l2 < 3); l2 = (l2 + 1)) lowpass_fRec0[l2] = 0.0;
#endif
}

inline void Dsp::init(uint32_t RsamplingFreq)
{
#ifndef  __MOD_DEVICES__
	sample_rate = 96000;
	smp.setup(RsamplingFreq, sample_rate);
	fSampleRate = sample_rate;
#else
	fSampleRate = RsamplingFreq;
#endif
	fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
	fConst1 = (3.68740366e-05f * fConst0);
	fConst2 = (0.0f - fConst1);
	fConst3 = mydsp_faustpower2_f(fConst0);
	float fConst4 = (2.50211268e-10f * fConst0);
	fConst5 = (fConst4 + -1.41841355e-07f);
	float fConst6 = (3.23341159e-10f * fConst0);
	fConst7 = (fConst6 + -1.83849806e-06f);
	fConst8 = (fConst4 + 1.41841355e-07f);
	fConst9 = (fConst6 + 1.83849806e-06f);
	clear_state_f();
}

void always_inline Dsp::compute(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0)
{
#define fVslider0 (*fVslider0_)
#define fHslider0 (*fHslider0_)
#ifndef  __MOD_DEVICES__
	FAUSTFLOAT buf[smp.max_out_count(count)];
	int ReCount = smp.up(count, input0, buf);
#else
	if(output0 != input0)
		memcpy(output0, input0, n_samples*sizeof(float));
	float* buf = output0;
	int ReCount = count;
#endif
	float fSlow0 = (0.000366769877f * (std::exp((3.0f * (1.0f - float(fVslider0)))) + -1.0f));
	float fSlow1 = (0.00700000022f * float(fHslider0));
	for (int i0 = 0; (i0 < ReCount); i0 = (i0 + 1)) {
		float fTemp0 = float(buf[i0]);
		float fTemp1 = float(rumorclip(float((0.666000009f * fRec0[1]))));
		float fTemp2 = (0.5f * fRec1[1]);
		float fTemp3 = (0.400000006f * fRec3[1]);
		float fTemp4 = (fTemp2 + ((0.300000012f * fRec5[1]) + fTemp3));
		float fTemp5 = (((0.200000003f * fRec7[1]) + fTemp1) - fTemp4);
		fRec7[0] = fTemp5;
		float fRec8 = (0.0f - (0.200000003f * fTemp5));
		fRec5[0] = (fRec8 + fRec7[1]);
		float fRec6 = (0.300000012f * (fTemp1 - fTemp4));
		fRec3[0] = (fRec6 + fRec5[1]);
		float fRec4 = (0.400000006f * (fTemp1 - (fTemp3 + fTemp2)));
		fRec1[0] = (fRec4 + fRec3[1]);
		float fRec2 = (0.5f * (fTemp1 - fTemp2));
		fRec10[0] = (fSlow0 + (0.992999971f * fRec10[1]));
		float fTemp6 = ((fConst0 * (fConst8 + (fConst9 * fRec10[0]))) + 9.19249069e-05f);
		fRec9[0] = (fTemp0 - (((fRec9[1] * ((fConst3 * ((0.0f - (6.46682319e-10f * fRec10[0])) + -5.00422537e-10f)) + 0.000183849814f)) + (fRec9[2] * ((fConst0 * (fConst5 + (fConst7 * fRec10[0]))) + 9.19249069e-05f))) / fTemp6));
		fRec0[0] = ((fTemp0 + (fRec2 + fRec1[1])) - float(rumorclip(float((((((fConst2 * fRec9[0]) + (6.87403781e-06f * fRec9[1])) + (fConst1 * fRec9[2])) / fTemp6) - fTemp0)))));
		fRec11[0] = (fSlow1 + (0.992999971f * fRec11[1]));
		buf[i0] = FAUSTFLOAT((fRec0[0] * fRec11[0]));
		fRec7[1] = fRec7[0];
		fRec5[1] = fRec5[0];
		fRec3[1] = fRec3[0];
		fRec1[1] = fRec1[0];
		fRec10[1] = fRec10[0];
		fRec9[2] = fRec9[1];
		fRec9[1] = fRec9[0];
		fRec0[1] = fRec0[0];
		fRec11[1] = fRec11[0];
	}
#ifndef  __MOD_DEVICES__
	for (int i0 = 0; (i0 < ReCount); i0 = (i0 + 1)) {
		double lowpass_fTemp0 = double(buf[i0]);
		lowpass_fVec0[0] = lowpass_fTemp0;
		lowpass_fRec1[0] = ((lowpass_fTemp0 + lowpass_fVec0[1]) - (0.99999999999999989 * lowpass_fRec1[1]));
		double lowpass_fTemp1 = (2.0 * lowpass_fRec0[1]);
		lowpass_fRec0[0] = (lowpass_fRec1[0] - (lowpass_fTemp1 + (0.99999999999999989 * lowpass_fRec0[2])));
		buf[i0] = float((lowpass_fRec0[2] + (lowpass_fRec0[0] + lowpass_fTemp1)));
		lowpass_fVec0[1] = lowpass_fVec0[0];
		lowpass_fRec1[1] = lowpass_fRec1[0];
		lowpass_fRec0[2] = lowpass_fRec0[1];
		lowpass_fRec0[1] = lowpass_fRec0[0];
	}
	smp.down(buf, output0);
#endif
#undef fVslider0
#undef fHslider0
}


void Dsp::connect(uint32_t port,void* data)
{
	switch ((PortIndex)port)
	{
	case INTENSITY: 
		fVslider0_ = static_cast<float*>(data); // , 0.5f, 0.0f, 1.0f, 0.00999999978f 
		break;
	case LEVEL: 
		fHslider0_ = static_cast<float*>(data); // , 0.5f, 0.0f, 1.0f, 0.00999999978f 
		break;
	default:
		break;
	}
}

Dsp *plugin() {
	return new Dsp();
}

void Dsp::del_instance(Dsp *p)
{
	delete p;
}
} // end namespace rumor
