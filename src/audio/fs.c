#include "audio.h"

#include <stdio.h>

static FILE *out_file = NULL;

static int aud_fs_init(int audioConfiguration, POPUS_MULTISTREAM_CONFIGURATION opusConfig, void* context, int arFlags) {
  const char* path = context;
  if (path != NULL && *path != '\0') {
    out_file = fopen(path, "wb");
    if (out_file == NULL) {
      fprintf(stderr, "Failed to open file for writing: %s\n", path);
      return -1;
    }
  } else {
    fprintf(stderr, "No output file specified. Will not write audio data.\n");
  }
  return 0;
}

static void aud_fs_cleanup(void) {
  if (out_file != NULL) {
    fclose(out_file);
    out_file = NULL;
  }
}

static void aud_fs_play(char* sampleData, int sampleLength) {
  if (fwrite(sampleData, 1, sampleLength, out_file) != sampleLength) {
    fprintf(stderr, "Failed to write to file\n");
  }
}

AUDIO_RENDERER_CALLBACKS audio_callbacks_filesystem = {
    .capabilities = CAPABILITY_DIRECT_SUBMIT,
    .init = aud_fs_init,
    .cleanup = aud_fs_cleanup,
    .decodeAndPlaySample = aud_fs_play,
};