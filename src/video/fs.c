#include "video.h"
#include <stdio.h>

static FILE *out_file = NULL;

static int vid_fs_setup(int videoFormat, int width, int height, int redrawRate, void* context, int drFlags) {
  const char* path = context;
  if (path != NULL && *path != '\0') {
    out_file = fopen(path, "wb");
    if (out_file == NULL) {
      fprintf(stderr, "Failed to open file for writing: %s\n", path);
      return -1;
    }
  } else {
    fprintf(stderr, "No output file specified. Will not write video data.\n");
  }
  return 0;
}

static void vid_fs_cleanup(void) {
  if (out_file != NULL) {
    fclose(out_file);
    out_file = NULL;
  }
}

static int vid_fs_submit(PDECODE_UNIT decodeUnit) {
  PLENTRY entry = decodeUnit->bufferList;
  while (entry != NULL) {
    if (out_file != NULL) {
      if (fwrite(entry->data, 1, entry->length, out_file) != entry->length) {
        fprintf(stderr, "Failed to write to file\n");
        return -1;
      }
    }
    entry = entry->next;
  }
  return 0;
}

DECODER_RENDERER_CALLBACKS decoder_callbacks_filesystem = {
  .capabilities = CAPABILITY_DIRECT_SUBMIT,
  .setup = vid_fs_setup,
  .cleanup = vid_fs_cleanup,
  .submitDecodeUnit = vid_fs_submit,
};