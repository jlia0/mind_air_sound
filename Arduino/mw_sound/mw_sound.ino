#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/cos8192_int8.h>
#include <mozzi_midi.h>

#define CONTROL_RATE 128
#define freq_interval 50


int count = 0;

// Data

int gamma[100] = {77, 231, 355, 143, 354, 421, 422, 386, 101, 206, 233, 460, 63, 218, 302, 247, 301, 71, 397, 198, 308, 200, 136, 455, 133, 255, 334, 323, 486, 237, 393, 452, 236, 424, 359, 466, 325, 412, 487, 381, 292, 315, 497, 232, 258, 121, 208, 472, 466, 208, 158, 148, 339, 494, 484, 578, 447, 160, 201, 83, 178, 137, 337, 424, 283, 167, 289, 125, 199, 88, 228, 347, 394, 224, 398, 488, 224, 401, 386, 218, 168, 139, 182, 197, 384, 128, 222, 161, 204, 135, 190, 250, 392, 245, 441, 363, 239, 405, 467, 267};
int delta[100] = {55, 357, 289, 197, 390, 372, 436, 331, 107, 180, 151, 199, 485, 325, 141, 177, 159, 185, 252, 409, 261, 147, 361, 422, 429, 279, 170, 391, 429, 264, 417, 384, 216, 186, 153, 364, 425, 241, 403, 145, 384, 296, 201, 431, 419, 458, 453, 33, 144, 219, 363, 243, 300, 172, 90, 312, 104, 231, 325, 310, 442, 415, 304, 334, 212, 126, 379, 294, 153, 273, 158, 174, 235, 213, 435, 212, 398, 471, 345, 321, 234, 389, 277, 454, 404, 203, 310, 149, 299, 127, 210, 12, 378, 226, 132, 242, 314, 300, 432, 264};
int beta[100] = {33, 426, 356, 146, 249, 454, 212, 185, 328, 268, 430, 262, 467, 303, 469, 228, 224, 272, 113, 340, 418, 304, 229, 434, 296, 311, 230, 319, 317, 407, 389, 488, 427, 313, 274, 190, 321, 476, 410, 217, 167, 304, 345, 449, 334, 145, 246, 368, 458, 384, 485, 308, 315, 400, 263, 199, 377, 384, 483, 283, 334, 141, 249, 148, 389, 431, 352, 457, 436, 226, 164, 193, 486, 134, 378, 270, 132, 345, 498, 451, 279, 481, 397, 233, 163, 395, 175, 406, 253, 385, 235, 385, 295, 406, 288, 327, 365, 281, 147, 290};
int alpha[100] = {88, 379, 291, 168, 210, 131, 226, 450, 187, 166, 179, 369, 253, 175, 178, 144, 235, 122, 299, 352, 456, 385, 263, 171, 310, 332, 138, 259, 380, 443, 223, 234, 287, 497, 234, 213, 367, 237, 325, 413, 293, 488, 376, 238, 273, 492, 445, 384, 211, 405, 299, 214, 156, 401, 116, 337, 256, 317, 218, 314, 301, 232, 145, 249, 124, 441, 354, 391, 263, 272, 163, 356, 389, 278, 261, 125, 106, 122, 240, 247, 121, 233, 203, 428, 368, 276, 381, 476, 447, 318, 214, 490, 352, 114, 371, 324, 474, 406, 479, 423};
int theta[100] = {55, 414, 321, 372, 418, 164, 411, 301, 332, 241, 471, 473, 419, 236, 312, 429, 316, 238, 300, 214, 116, 331, 265, 269, 426, 273, 327, 220, 229, 205, 380, 312, 299, 361, 457, 291, 412, 388, 497, 298, 142, 291, 377, 270, 340, 284, 225, 101, 233, 340, 208, 144, 238, 318, 288, 171, 303, 286, 378, 435, 391, 271, 329, 284, 432, 372, 238, 193, 231, 354, 170, 115, 242, 491, 207, 358, 423, 345, 270, 187, 401, 268, 415, 236, 249, 345, 407, 467, 371, 281, 453, 411, 219, 270, 458, 228, 297, 393, 448, 253};


// harmonics
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos1(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos2(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos3(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos4(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos5(COS8192_DATA);


// volume controls
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kVol1(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kVol2(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kVol3(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kVol4(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kVol5(COS8192_DATA);


// audio volumes updated each control interrupt and reused in audio till next control
char v1, v2, v3, v4, v5, v6, v7, v8;

void setup() {

  // set harmonic frequencies
  aCos1.setFreq(mtof(beta[0]));
  aCos2.setFreq(mtof(alpha[0]));
  aCos3.setFreq(mtof(theta[0]));
  aCos4.setFreq(mtof(delta[0]));
  aCos5.setFreq(mtof(gamma[0]));

  // set volume change frequencies

  kVol1.setFreq(1.2f); // beta
  kVol2.setFreq(0.8f); // alpha
  kVol3.setFreq(0.4f); // theta
  kVol4.setFreq(0.05f); // delta
  kVol5.setFreq(0.5f); // gamma

  //  kVol2.setFreq(0.0245f);
  //  kVol3.setFreq(0.019f);
  //  kVol4.setFreq(0.07f);
  //  kVol5.setFreq(0.047f);


  v1 = v2 = v3 = v4 = v5 = 127;

  startMozzi(CONTROL_RATE);
}

void loop() {
  audioHook();
}


void updateControl() {
  v1 = kVol1.next() >> 1; // going at a higher freq, this creates zipper noise, so reduce the gain
  v2 = kVol2.next();
  v3 = kVol3.next();
  v4 = kVol4.next();
  v5 = kVol5.next();
//
  if (count < 100) {
    aCos1.setFreq(beta[count] - 200);
    aCos2.setFreq(alpha[count] - 220);
    aCos3.setFreq(theta[count] - 240);
    aCos4.setFreq(delta[count] - 270);
    aCos5.setFreq(gamma[count] - 300);
  }

  count ++;

  if (count == 100){
    count = 0;
  }
  
//
//
  delay(freq_interval);

}

int updateAudio() {
  long asig = (long)
              aCos1.next() * v1 +
              aCos2.next() * v2 +
              aCos3.next() * v3 +
              aCos4.next() * v4 +
              aCos5.next() * v5;
  asig >>= 9;
  return (int) asig;
}
