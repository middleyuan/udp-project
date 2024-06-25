#pragma once
#include <cstdint>
#include <cstring>

struct Message {
    // use uint32_t to ensure 4-byte alignment accorss platforms
    // many network protocols require data to be aligned to 4 bytes
    uint32_t sequence_number; 
    char data[256];

    Message() : sequence_number(0) {
        // initialize data with zeros
        std::memset(data, 0, sizeof(data));
    }

    Message(uint32_t seq, const char* msg) : sequence_number(seq) {
        // copy msg to data, ensuring null-termination
        // common library functions for strings expect null-terminated strings
        std::strncpy(data, msg, sizeof(data) - 1);
        data[sizeof(data) - 1] = '\0';
    }
};