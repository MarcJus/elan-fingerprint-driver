//
// Created by marc on 09/03/23.
//

#ifndef FINGERPRINT_READER_FINGERPRINT_H
#define FINGERPRINT_READER_FINGERPRINT_H

struct fingerprint_message_header {

    unsigned short type;

    unsigned short length;
};

// Type
#define FINGERPRINT_RECOGNIZED 0x1
#define FINGERPRINT_TIMEOUT 0x6e
#define FINGERPRINT_UNRECOGNIZED 0xfd

#endif //FINGERPRINT_READER_FINGERPRINT_H
