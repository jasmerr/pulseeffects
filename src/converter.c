#include <stdio.h>
#define NUMBANDS  22
#define GAP  10
#define Q_CONSTANT 400

struct band {
  float gain;
  float frequency;
  float width;
};
void add_beginning(FILE *beginning, FILE *output)
{
  char c;
  // copy the non eq parts of the preset into the output
  while ((c = fgetc(beginning)) != EOF) {
    fputc(c, output);
  }
  fprintf(output, "        \"equalizer\": {");
  fprintf(output, "\n            \"state\": \"true\",");
  fprintf(output, "\n            \"num-bands\": \"%d\",", NUMBANDS);
  fprintf(output, "\n            \"input-gain\": \"0\",");
  fprintf(output, "\n            \"output-gain\": \"0\",");
}

void add_band(FILE *output, int bandNum, struct band this_band, int end)
 {
  fprintf(output, "\n            \"band%d\": {", bandNum);
  if (this_band.gain >= 12) {
    fprintf(output, "\n                \"gain\": \"%f\",", (float)12);
  }
  else if (this_band.gain <=-24) {
    fprintf(output, "\n                \"gain\": \"%f\",", (float)-24);
  }
  else {
    fprintf(output, "\n                \"gain\": \"%f\",", this_band.gain);
  }
  fprintf(output, "\n                \"gain\": \"%f\",", this_band.gain);
  fprintf(output, "\n                \"frequency\": \"%f\",", this_band.frequency);
  fprintf(output, "\n                \"width\": \"%f\",", this_band.width);
  fprintf(output, "\n                \"type\": \"peak\"");
  if (end) {
    fprintf(output, "\n            }");
  }
  else {
    fprintf(output, "\n           },");
  }
}

void end_file(FILE *end, FILE *output)
{
  char c;
  fprintf(output, "\n        },");
  while ((c = fgetc(end)) != EOF) {
    fputc(c, output);
  }
  // fprintf(output, "\n }\n}");
}

int read_bands(FILE *in, struct band bands[], int *counter)
{
  float freq, gain;
  // skip 2 lines
  fscanf(in, "%*[^\n]\n", NULL);
  fscanf(in, "%*[^\n]\n", NULL);

  while (*counter < 230 && fscanf(in,"%f %f", &freq, &gain) == 2) {
    struct band bandTemp;
    bandTemp.gain = gain;
    bandTemp.frequency = freq;
    bandTemp.width = Q_CONSTANT;
    bands[*counter] = bandTemp;
    printf("\nInter num %d, %f %f", *counter, bandTemp.frequency, bandTemp.gain);
    (*counter)++;
  }
}

void main()
{
  int counter = 0, counter2 = 0;
  FILE *frd, *beginning, *end, *preset;
  struct band bands[230];

  frd = fopen("Volume40FeetTouchEQ.frd", "r");
  beginning = fopen("baseline_beginning.json", "r");
  end = fopen("baseline_ending.json", "r");
  preset = fopen ("preset.json", "w");

  add_beginning(beginning, preset);
  read_bands(frd, bands, &counter);
  for (int i = 0; i < 220; i = i + GAP)
  {
    if (i + GAP < 220) {
      add_band(preset, counter2, bands[i], 0);
    }
    else {
      add_band(preset, counter2, bands[i], 1);
    }
    counter2++;
  }
  end_file(end, preset);

  fclose(frd);
  fclose(preset);
}
