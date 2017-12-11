#include "cxdec.h"

static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage);
static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage);

static BYTE EncryptionControlBlock[4096] = {
		0x20, 0x45, 0x6e, 0x63, 0x72, 0x79, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x63, 0x6f, 0x6e, 0x74, 
		0x72, 0x6f, 0x6c, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x20, 0x2d, 0x2d, 0x20, 0x53, 0x74, 0x61, 
		0x74, 0x69, 0x63, 0x61, 0x6c, 0x6c, 0x79, 0x20, 0x6f, 0x72, 0x20, 0x64, 0x79, 0x6e, 0x61, 0x6d, 
		0x69, 0x63, 0x61, 0x6c, 0x6c, 0x79, 0x2c, 0x20, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6c, 0x79, 
		0x20, 0x6f, 0x72, 0x20, 0x69, 0x6e, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6c, 0x79, 0x2c, 0x20, 
		0x75, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 
		0x61, 0x6d, 0x20, 0x61, 0x6e, 0x64, 0x2f, 0x6f, 0x72, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x20, 
		0x66, 0x72, 0x6f, 0x6d, 0x20, 0x6f, 0x74, 0x68, 0x65, 0x72, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 
		0x61, 0x6d, 0x73, 0x20, 0x77, 0x69, 0x6c, 0x6c, 0x20, 0x62, 0x65, 0x20, 0x69, 0x6c, 0x6c, 0x65, 
		0x67, 0x61, 0x6c, 0x20, 0x62, 0x79, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6c, 0x69, 0x63, 0x65, 0x6e, 
		0x73, 0x65, 0x20, 0x61, 0x67, 0x72, 0x65, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x20, 0x82, 0xb1, 
		0x82, 0xcc, 0x83, 0x76, 0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 0x82, 0xe2, 0x83, 0x75, 
		0x83, 0x8d, 0x83, 0x62, 0x83, 0x4e, 0x82, 0xf0, 0x81, 0x41, 0x90, 0xc3, 0x93, 0x49, 0x82, 0xc5, 
		0x82, 0xa0, 0x82, 0xea, 0x93, 0xae, 0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x81, 0x41, 
		0x92, 0xbc, 0x90, 0xda, 0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x8a, 0xd4, 0x90, 0xda, 
		0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x81, 0x41, 0x91, 0xbc, 0x82, 0xcc, 0x83, 0x76, 
		0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 0x82, 0xa9, 0x82, 0xe7, 0x97, 0x70, 0x82, 0xa2, 
		0x82, 0xe9, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xcd, 0x83, 0x89, 0x83, 0x43, 0x83, 0x5a, 0x83, 0x93, 
		0x83, 0x58, 0x82, 0xc9, 0x82, 0xe6, 0x82, 0xe8, 0x8b, 0xd6, 0x82, 0xb6, 0x82, 0xe7, 0x82, 0xea, 
		0x82, 0xc4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x88, 0xea, 0x8f, 0x75, 0x92, 
		0x78, 0x82, 0xea, 0x82, 0xc4, 0x81, 0x41, 0x94, 0x6e, 0x81, 0x7e, 0x74, 0x6a, 0x73, 0x82, 0xc8, 
		0x82, 0xf1, 0x82, 0xc4, 0x82, 0xcc, 0x82, 0xcd, 0x88, 0xa4, 0x82, 0xbe, 0x82, 0xa9, 0x82, 0xe7, 
		0x82, 0xc8, 0x81, 0x42, 0x96, 0x6c, 0x82, 0xcd, 0x81, 0x41, 0x8f, 0xb8, 0x8d, 0x7e, 0x8c, 0xfb, 
		0x82, 0xcc, 0x83, 0x47, 0x83, 0x8c, 0x83, 0x78, 0x81, 0x5b, 0x83, 0x5e, 0x81, 0x5b, 0x82, 0xcc, 
		0x83, 0x58, 0x83, 0x43, 0x83, 0x62, 0x83, 0x60, 0x82, 0xf0, 0x89, 0x9f, 0x82, 0xb5, 0x82, 0xbd, 
		0x81, 0x42, 0x83, 0x4a, 0x83, 0x60, 0x83, 0x83, 0x81, 0x41, 0x83, 0x4a, 0x83, 0x60, 0x83, 0x83, 
		0x82, 0xc6, 0x8c, 0xbe, 0x82, 0xa4, 0x81, 0x41, 0x82, 0xde, 0x82, 0xc8, 0x82, 0xb5, 0x82, 0xa2, 
		0x83, 0x58, 0x83, 0x43, 0x83, 0x62, 0x83, 0x60, 0x82, 0xcc, 0x89, 0xb9, 0x82, 0xaa, 0x93, 0x57, 
		0x96, 0x5d, 0x91, 0xe4, 0x82, 0xcc, 0x92, 0x86, 0x82, 0xc9, 0x8b, 0xbf, 0x82, 0xad, 0x81, 0x42, 
		0x81, 0x63, 0x93, 0xae, 0x82, 0xa9, 0x82, 0xc8, 0x82, 0xa2, 0x81, 0x42, 0x8c, 0x78, 0x8e, 0xe8, 
		0x82, 0xaa, 0x92, 0x4e, 0x82, 0xe0, 0x82, 0xa2, 0x82, 0xc8, 0x82, 0xa2, 0x82, 0xc6, 0x8a, 0xa8, 
		0x88, 0xe1, 0x82, 0xa2, 0x82, 0xb5, 0x82, 0xbd, 0x82, 0xcc, 0x82, 0xa9, 0x81, 0x41, 0x83, 0x47, 
		0x83, 0x8c, 0x83, 0x78, 0x81, 0x5b, 0x83, 0x5e, 0x81, 0x5b, 0x82, 0xcc, 0x93, 0x64, 0x8c, 0xb9, 
		0x82, 0xf0, 0x90, 0xd8, 0x82, 0xc1, 0x82, 0xc4, 0x82, 0xb5, 0x82, 0xdc, 0x82, 0xc1, 0x82, 0xbd, 
		0x82, 0xe7, 0x82, 0xb5, 0x82, 0xa2, 0x81, 0x42, 0x82, 0xa0, 0x82, 0xed, 0x82, 0xc4, 0x82, 0xc4, 
		0x93, 0x57, 0x96, 0x5d, 0x91, 0xe4, 0x82, 0xa9, 0x82, 0xe7, 0x89, 0xba, 0x82, 0xf0, 0x8c, 0xa9, 
		0x89, 0xba, 0x82, 0xeb, 0x82, 0xb7, 0x82, 0xc6, 0x81, 0x41, 0x8a, 0xe7, 0x82, 0xc8, 0x82, 0xb6, 
		0x82, 0xdd, 0x82, 0xcc, 0x8c, 0x78, 0x8e, 0xe8, 0x82, 0xaa, 0x81, 0x41, 0x82, 0xb1, 0x82, 0xcc, 
		0x83, 0x5e, 0x83, 0x8f, 0x81, 0x5b, 0x82, 0xf0, 0x8b, 0x8e, 0x82, 0xc1, 0x82, 0xc4, 0x8d, 0x73, 
		0x82, 0xad, 0x82, 0xc6, 0x82, 0xb1, 0x82, 0xeb, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xc1, 0x82, 0xbd, 
		0x81, 0x42, 0x0a, 0x0a, 0x81, 0x40, 0x81, 0x75, 0x82, 0xdc, 0x82, 0xa2, 0x82, 0xc1, 0x82, 0xa9, 
		0x81, 0x63, 0x81, 0x76, 0x96, 0x6c, 0x82, 0xcd, 0x81, 0x41, 0x82, 0xbb, 0x82, 0xf1, 0x82, 0xc8, 
		0x93, 0xc6, 0x82, 0xe8, 0x8c, 0xbe, 0x82, 0xf0, 0x8c, 0xbe, 0x82, 0xa2, 0x82, 0xc8, 0x82, 0xaa, 
		0x82, 0xe7, 0x81, 0x41, 0x82, 0xdc, 0x82, 0xbd, 0x81, 0x41, 0x82, 0xb3, 0x82, 0xc1, 0x82, 0xab, 
		0x82, 0xcc, 0x82, 0xc6, 0x82, 0xb1, 0x82, 0xeb, 0x82, 0xc9, 0x8d, 0x98, 0x8a, 0x7c, 0x82, 0xaf, 
		0x81, 0x41, 0x94, 0xbc, 0x95, 0xaa, 0x82, 0xd9, 0x82, 0xc7, 0x92, 0xbe, 0x82, 0xf1, 0x82, 0xbe, 
		0x81, 0x41, 0x91, 0xe5, 0x82, 0xab, 0x82, 0xc8, 0x97, 0x5b, 0x93, 0xfa, 0x82, 0xf0, 0x97, 0x4a, 
		0x9f, 0x54, 0x82, 0xb0, 0x82, 0xc9, 0x81, 0x41, 0x96, 0x6a, 0x8f, 0xf1, 0x82, 0xf0, 0x82, 0xc2, 
		0x82, 0xa2, 0x82, 0xc4, 0x81, 0x41, 0x92, 0xad, 0x82, 0xdf, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xbd, 
		0x81, 0x42, 0x0a, 0x0a, 0x81, 0x40, 0x89, 0xbd, 0x95, 0xaa, 0x97, 0xa7, 0x82, 0xc1, 0x82, 0xbd, 
		0x82, 0xeb, 0x82, 0xa4, 0x82, 0xa9, 0x81, 0x41, 0x91, 0xbe, 0x97, 0x7a, 0x82, 0xcd, 0x81, 0x41, 
		0x8d, 0xc5, 0x8c, 0xe3, 0x82, 0xcc, 0x8e, 0x63, 0x8c, 0xf5, 0x82, 0xf0, 0x8b, 0xf3, 0x82, 0xc9, 
		0x95, 0xfa, 0x82, 0xc1, 0x82, 0xbd, 0x82, 0xa0, 0x82, 0xc6, 0x81, 0x41, 0x91, 0xe5, 0x8a, 0x43, 
		0x82, 0xcc, 0x92, 0xea, 0x82, 0xc9, 0x92, 0xbe, 0x82, 0xdd, 0x82, 0xab, 0x82, 0xe8, 0x81, 0x41, 
		0x8a, 0x43, 0x82, 0xcd, 0x90, 0xc3, 0x8e, 0xe2, 0x82, 0xf0, 0x8e, 0xe6, 0x82, 0xe8, 0x96, 0xdf, 
		0x82, 0xb5, 0x82, 0xbd, 0x81, 0x42, 0x82, 0xde, 0x82, 0xeb, 0x82, 0xf1, 0x81, 0x41, 0x93, 0x57, 
		0x96, 0x5d, 0x91, 0xe4, 0x82, 0xc9, 0x89, 0xb9, 0x82, 0xf0, 0x94, 0xad, 0x82, 0xb7, 0x82, 0xe9, 
		0x82, 0xe0, 0x82, 0xcc, 0x82, 0xcd, 0x82, 0xc8, 0x82, 0xa2, 0x81, 0x42, 0x82, 0xbd, 0x82, 0xbe, 
		0x81, 0x41, 0x8a, 0x4f, 0x82, 0xc9, 0x82, 0xa0, 0x82, 0xe9, 0x82, 0xa2, 0x82, 0xad, 0x82, 0xc2, 
		0x82, 0xa9, 0x82, 0xcc, 0x96, 0xd8, 0x81, 0x58, 0x82, 0xcc, 0x97, 0x74, 0x82, 0xcc, 0x82, 0xb1, 
		0x82, 0xb7, 0x82, 0xea, 0x82, 0xe9, 0x89, 0xb9, 0x82, 0xaa, 0x81, 0x41, 0x82, 0xb3, 0x81, 0x5b, 
		0x81, 0x41, 0x82, 0xc6, 0x95, 0xb7, 0x82, 0xb1, 0x82, 0xa6, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xc4, 
		0x81, 0x63, 0x8c, 0xe3, 0x82, 0xcd, 0x81, 0x41, 0x93, 0xe0, 0x82, 0xc8, 0x82, 0xe9, 0x8c, 0xc4, 
		0x8b, 0x7a, 0x82, 0xcc, 0x89, 0xb9, 0x82, 0xc6, 0x81, 0x41, 0x8e, 0x9e, 0x81, 0x58, 0x82, 0xcc, 
		0x96, 0x6c, 0x82, 0xcc, 0x82, 0xbd, 0x82, 0xdf, 0x91, 0xa7, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xeb, 
		0x82, 0xa4, 0x82, 0xa9, 0x81, 0x42, 0x0a, 0x0a, 0x81, 0x40, 0x82, 0xbb, 0x82, 0xcc, 0x82, 0xdc, 
		0x82, 0xdc, 0x81, 0x41, 0x82, 0xc8, 0x82, 0xc9, 0x82, 0xf0, 0x8c, 0xa9, 0x82, 0xc4, 0x82, 0xa2, 
		0x82, 0xe9, 0x82, 0xcc, 0x82, 0xa9, 0x82, 0xcd, 0x8e, 0xa9, 0x95, 0xaa, 0x82, 0xc9, 0x82, 0xcd, 
		0x8b, 0x4c, 0x89, 0xaf, 0x82, 0xaa, 0x82, 0xc8, 0x82, 0xa2, 0x81, 0x42, 0x82, 0xa8, 0x82, 0xbb, 
		0x82, 0xe7, 0x82, 0xad, 0x81, 0x41, 0x91, 0xbe, 0x97, 0x7a, 0x82, 0xaa, 0x92, 0xbe, 0x82, 0xf1, 
		0x82, 0xc5, 0x82, 0xe0, 0x81, 0x41, 0x82, 0xbb, 0x82, 0xcc, 0x91, 0xbe, 0x97, 0x7a, 0x82, 0xaa, 
		0x92, 0xbe, 0x82, 0xf1, 0x82, 0xbe, 0x8f, 0xea, 0x8f, 0x8a, 0x82, 0xcc, 0x8a, 0x43, 0x82, 0xf0, 
		0x82, 0xb8, 0x82, 0xc1, 0x82, 0xc6, 0x8c, 0xa9, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xbd, 0x82, 0xc9, 
		0x88, 0xe1, 0x82, 0xa2, 0x82, 0xc8, 0x82, 0xa2, 0x81, 0x42, 0x93, 0x94, 0x91, 0xe4, 0x82, 0xcc, 
		0x8c, 0xf5, 0x82, 0xaa, 0x8e, 0x9e, 0x81, 0x58, 0x82, 0xb1, 0x82, 0xb1, 0x82, 0xcc, 0x93, 0x57, 
		0x96, 0x5d, 0x91, 0xe4, 0x93, 0xe0, 0x82, 0xf0, 0x8f, 0xc6, 0x82, 0xe7, 0x82, 0xb7, 0x88, 0xc8, 
		0x8a, 0x4f, 0x81, 0x41, 0x82, 0xb1, 0x82, 0xcc, 0x93, 0x57, 0x96, 0x5d, 0x91, 0xe4, 0x93, 0xe0, 
		0x82, 0xf0, 0x8f, 0xc6, 0x82, 0xe7, 0x82, 0xb7, 0x82, 0xcc, 0x82, 0xcd, 0x81, 0x41, 0x96, 0x9e, 
		0x8c, 0x8e, 0x82, 0xc9, 0x8b, 0xdf, 0x82, 0xa2, 0x8c, 0x8e, 0x82, 0xbe, 0x82, 0xaf, 0x82, 0xc5, 
		0x82, 0xa0, 0x82, 0xe9, 0x81, 0x42, 0x82, 0xbd, 0x82, 0xbe, 0x81, 0x41, 0x8a, 0x43, 0x82, 0xe0, 
		0x81, 0x41, 0x82, 0xdc, 0x82, 0xe9, 0x82, 0xc5, 0x8e, 0xa9, 0x82, 0xe7, 0x8c, 0xf5, 0x82, 0xf0, 
		0x94, 0xad, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x82, 0xa9, 0x82, 0xcc, 0x82, 0xe6, 
		0x82, 0xa4, 0x82, 0xc9, 0x81, 0x41, 0x82, 0xda, 0x82, 0xf1, 0x82, 0xe2, 0x82, 0xe8, 0x82, 0xc6, 
		0x81, 0x41, 0x82, 0xa4, 0x82, 0xc2, 0x82, 0xeb, 0x82, 0xc9, 0x8c, 0xf5, 0x82, 0xc1, 0x82, 0xc4, 
		0x82, 0xa2, 0x82, 0xbd, 0x81, 0x42, 0x0a, 0x66, 0xef, 0x6f, 0x2a, 0x50, 0x12, 0x0d, 0xcc, 0x2d, 
		0xfc, 0x66, 0x88, 0x27, 0x05, 0x31, 0x4c, 0x72, 0x49, 0xdd, 0x2c, 0xf2, 0xff, 0xe7, 0x9b, 0x14, 
		0x81, 0x33, 0xfb, 0xb0, 0xa9, 0x6c, 0xf4, 0x6a, 0xa3, 0x2a, 0xa5, 0x9d, 0x39, 0xc2, 0x9c, 0xd1, 
		0x1c, 0x69, 0xea, 0x89, 0x3c, 0x2d, 0x38, 0x56, 0xe7, 0xb2, 0x63, 0x8d, 0x45, 0x7b, 0x5b, 0x47, 
		0x58, 0x3c, 0x03, 0x2e, 0x76, 0x14, 0x72, 0x98, 0x05, 0x63, 0x0c, 0xbd, 0x90, 0xe8, 0x9f, 0x30, 
		0x17, 0x4a, 0xbc, 0x94, 0xa9, 0xfc, 0x1f, 0xea, 0x26, 0xa0, 0x42, 0x11, 0x84, 0x71, 0x3f, 0xf0, 
		0x23, 0x9d, 0xa5, 0x9c, 0x03, 0x61, 0x5f, 0xec, 0x06, 0x4a, 0xfe, 0x4f, 0x27, 0x77, 0xed, 0x05, 
		0x4b, 0x34, 0x66, 0x66, 0x9c, 0x2d, 0xdc, 0x6c, 0xce, 0xd8, 0x54, 0x30, 0x8a, 0xc3, 0x0a, 0xd0, 
		0x68, 0x2e, 0x7f, 0x7c, 0x89, 0x47, 0x4f, 0x37, 0x29, 0x62, 0x6f, 0x04, 0x27, 0xbe, 0xf2, 0xcf, 
		0xe9, 0x91, 0x4f, 0xf5, 0xe9, 0x67, 0xed, 0x6c, 0x7f, 0xb2, 0xb7, 0xaf, 0x26, 0x3d, 0xd0, 0xe6, 
		0x73, 0x9f, 0x05, 0x53, 0x46, 0x4f, 0xdd, 0xc9, 0xc7, 0x04, 0xd9, 0xe7, 0x08, 0x4a, 0xeb, 0x22, 
		0x35, 0x54, 0x04, 0x48, 0x67, 0x33, 0x39, 0xfe, 0x69, 0x57, 0x1d, 0x98, 0xad, 0xd4, 0xf9, 0x49, 
		0x32, 0x74, 0x2f, 0xad, 0x8b, 0x35, 0x75, 0xfa, 0xfc, 0xcb, 0xc5, 0x80, 0x5b, 0x02, 0xeb, 0x66, 
		0x49, 0xb1, 0xf8, 0xfb, 0xd0, 0x30, 0x66, 0xbd, 0x94, 0x34, 0x7e, 0x05, 0x08, 0x7e, 0x42, 0x31, 
		0xba, 0x42, 0xb6, 0x97, 0xfe, 0xb5, 0x5a, 0xf4, 0x27, 0xa8, 0x67, 0x30, 0x00, 0x1c, 0x8a, 0x88, 
		0x51, 0x30, 0xe0, 0x00, 0xc7, 0x7d, 0x20, 0x92, 0x99, 0x0c, 0x51, 0x6c, 0x72, 0x65, 0x6d, 0xee, 
		0xe0, 0x36, 0xbb, 0x73, 0x58, 0xb4, 0xa1, 0xf7, 0x4c, 0x2d, 0xf1, 0xe9, 0xef, 0xe6, 0x22, 0x44, 
		0x00, 0x7f, 0xf3, 0xc9, 0xed, 0x01, 0x18, 0xd0, 0xe6, 0x76, 0x68, 0xbe, 0xbd, 0x47, 0xfe, 0x8f, 
		0x71, 0x7d, 0x01, 0x07, 0x58, 0x6b, 0x34, 0x03, 0xc6, 0xc4, 0x5d, 0xe4, 0xa3, 0x41, 0x71, 0xc0, 
		0x89, 0x1f, 0x11, 0x9f, 0x69, 0xa5, 0x3f, 0x1c, 0xad, 0x49, 0x63, 0x46, 0x8d, 0xc7, 0xdc, 0xed, 
		0x49, 0xdb, 0xcf, 0x6f, 0xbe, 0x22, 0xd1, 0xb1, 0xec, 0x18, 0x35, 0xe9, 0x1a, 0x10, 0x8c, 0xf4, 
		0x06, 0x54, 0xab, 0x5f, 0x79, 0xe6, 0x6e, 0x1e, 0x65, 0x35, 0x39, 0xe1, 0x9b, 0x9c, 0x48, 0x1f, 
		0xae, 0xa6, 0xb1, 0x46, 0x2f, 0x3d, 0x3b, 0xb9, 0xb3, 0xd9, 0x1a, 0xe9, 0xe6, 0x4c, 0x47, 0xdb, 
		0x25, 0x38, 0x65, 0x28, 0xab, 0xa5, 0x22, 0x83, 0x26, 0x3c, 0xb3, 0xb0, 0xa4, 0x7f, 0x3e, 0x23, 
		0x66, 0x3a, 0x77, 0x81, 0x35, 0x1b, 0x76, 0xed, 0x0c, 0xfb, 0x41, 0xe7, 0xee, 0x02, 0x0f, 0x76, 
		0xd4, 0x07, 0x79, 0x2e, 0xa2, 0x5a, 0xdb, 0x4b, 0x30, 0xa3, 0x71, 0xe8, 0x66, 0x3f, 0x57, 0xdc, 
		0x0b, 0x05, 0xf1, 0x25, 0xe0, 0xe7, 0xec, 0xde, 0xbf, 0x45, 0x24, 0x2b, 0xad, 0x41, 0x12, 0xc7, 
		0xdc, 0x53, 0xc4, 0x7a, 0x88, 0x7b, 0x5a, 0x5a, 0x85, 0x04, 0x15, 0xe2, 0x08, 0xbf, 0xe9, 0x1e, 
		0x0f, 0x8b, 0x19, 0xfc, 0x3e, 0x99, 0x5c, 0xb3, 0x3a, 0xa4, 0xbb, 0xc0, 0xf0, 0x2f, 0xd2, 0xa5, 
		0x43, 0xa1, 0xa8, 0x18, 0x05, 0x96, 0xc9, 0xeb, 0x69, 0x4d, 0x54, 0x3d, 0xc3, 0xb3, 0xd2, 0xc4, 
		0x61, 0x52, 0xc9, 0xe4, 0xf6, 0x53, 0xc5, 0x66, 0x2c, 0xd8, 0xa9, 0x43, 0x1c, 0x03, 0xb7, 0xcd, 
		0xd8, 0xb7, 0xca, 0xe8, 0x5b, 0xcf, 0x25, 0x38, 0x78, 0x31, 0x0a, 0x16, 0x90, 0xa9, 0x11, 0x8a, 
		0x8b, 0x75, 0x23, 0x91, 0x33, 0xb8, 0xc1, 0xf6, 0x88, 0xe4, 0xa3, 0xab, 0x15, 0x7a, 0xec, 0x4e, 
		0x89, 0xc6, 0x62, 0xad, 0x9d, 0xc9, 0xdb, 0x04, 0x91, 0x34, 0x94, 0xf7, 0xc4, 0x6d, 0x0d, 0x04, 
		0x66, 0x23, 0x46, 0x37, 0x20, 0xee, 0xf6, 0xe7, 0x18, 0x20, 0x5e, 0x6b, 0xc7, 0x64, 0x9e, 0xbd, 
		0x6e, 0x8c, 0x0e, 0x82, 0x41, 0xbe, 0x2f, 0x62, 0xd3, 0x80, 0x15, 0xe7, 0xf0, 0xa3, 0xef, 0xc6, 
		0xe4, 0xdf, 0xad, 0x60, 0xd0, 0x41, 0x24, 0x8d, 0x36, 0x39, 0x6a, 0xd1, 0x31, 0x76, 0x51, 0xb1, 
		0x60, 0x50, 0xc2, 0x80, 0x56, 0x7d, 0x4a, 0xfc, 0x85, 0x95, 0x43, 0xb2, 0x3e, 0x40, 0xe2, 0xb9, 
		0x75, 0x80, 0xa0, 0x2c, 0xb8, 0xd1, 0xcc, 0xa4, 0x94, 0x9e, 0xa5, 0xbb, 0x23, 0xf9, 0xdc, 0x14, 
		0x7a, 0x19, 0xdf, 0x4a, 0x58, 0xa3, 0x19, 0x3e, 0x12, 0x33, 0x69, 0x4a, 0x3b, 0xff, 0xe7, 0xad, 
		0xe1, 0xa0, 0x42, 0xcc, 0x52, 0x82, 0xf4, 0xbc, 0x74, 0x93, 0x44, 0x1b, 0x86, 0x37, 0xe9, 0xb8, 
		0x2f, 0xc4, 0x20, 0x82, 0x5c, 0xab, 0x09, 0xa8, 0xa8, 0xa1, 0x64, 0x36, 0xf7, 0x1f, 0xd3, 0x73, 
		0x93, 0x2f, 0x33, 0x47, 0x9a, 0x3b, 0xf6, 0xe6, 0x6f, 0x31, 0xcf, 0x3b, 0x14, 0x44, 0x77, 0xbf, 
		0x03, 0xd3, 0x23, 0xa1, 0xda, 0x4b, 0x5f, 0x0f, 0x38, 0xe9, 0x57, 0xbd, 0x06, 0xf1, 0x81, 0x1d, 
		0xbb, 0xbf, 0xab, 0x25, 0xa4, 0x75, 0xfe, 0x39, 0x63, 0xd2, 0xa3, 0x64, 0x1b, 0x81, 0x59, 0xd2, 
		0xcf, 0x9b, 0x98, 0x83, 0xce, 0x11, 0x33, 0x9c, 0x89, 0xe8, 0x68, 0x6c, 0x61, 0x2b, 0x0a, 0xea, 
		0xcb, 0xb8, 0x87, 0x46, 0xd4, 0x12, 0x90, 0x9f, 0x6c, 0x26, 0x20, 0xb6, 0xbd, 0xa9, 0x67, 0x9c, 
		0xb1, 0xb5, 0xf7, 0x59, 0x35, 0x5e, 0xf0, 0x8c, 0xa6, 0x1f, 0xe1, 0xe5, 0x16, 0x1a, 0xf1, 0xd6, 
		0x0a, 0xf6, 0xa5, 0x9d, 0xa4, 0x05, 0x81, 0x18, 0xd0, 0xb3, 0xea, 0xe4, 0xb2, 0x36, 0xb9, 0x9c, 
		0x89, 0x0c, 0x35, 0x55, 0xf4, 0x59, 0x57, 0x5e, 0x29, 0xe6, 0x22, 0x6d, 0xa3, 0x0b, 0xdb, 0xe0, 
		0xd4, 0x14, 0xa8, 0xde, 0x92, 0xc6, 0x7b, 0xe6, 0xaa, 0x0f, 0xba, 0x35, 0x35, 0xfb, 0xd0, 0xe8, 
		0xdb, 0xe3, 0x23, 0x81, 0x04, 0xf9, 0x7b, 0xa9, 0x19, 0x57, 0x0b, 0x5e, 0x9d, 0xdf, 0x70, 0xf0, 
		0xd7, 0x18, 0x49, 0xd4, 0x1e, 0x8f, 0x7b, 0x60, 0x10, 0xe7, 0xdb, 0x45, 0x82, 0x1a, 0xff, 0xd4, 
		0x04, 0xbe, 0x60, 0x79, 0x9d, 0x0f, 0xc3, 0xd4, 0xf9, 0x5d, 0x54, 0xd4, 0xfe, 0x8f, 0x1c, 0xc0, 
		0x87, 0x18, 0xd0, 0x6d, 0x2e, 0xa0, 0x53, 0xff, 0x38, 0x6a, 0x2d, 0xb1, 0xed, 0x4c, 0x5a, 0xcf, 
		0xdc, 0xf5, 0xe3, 0x1d, 0x53, 0xa8, 0x6c, 0x9c, 0x54, 0xbe, 0x86, 0xb1, 0x11, 0x14, 0x63, 0x0d, 
		0x19, 0x7e, 0xa0, 0xdd, 0x0c, 0x4c, 0xda, 0xd4, 0xd2, 0x5c, 0xa6, 0x6e, 0x9d, 0xdf, 0x0f, 0xe5, 
		0xd8, 0xb8, 0xf3, 0x60, 0x76, 0x2d, 0xb7, 0x53, 0xc7, 0xfd, 0x00, 0x69, 0x37, 0x7c, 0x06, 0x19, 
		0xea, 0x0c, 0xa9, 0xd7, 0x06, 0xcb, 0xa8, 0xb0, 0x67, 0x96, 0xce, 0x84, 0x4c, 0xa2, 0x8b, 0x3f, 
		0x3a, 0x74, 0x30, 0x5c, 0x6f, 0x55, 0x25, 0x8b, 0x45, 0x74, 0x25, 0xc6, 0xd6, 0xeb, 0x1f, 0x78, 
		0xf8, 0xc1, 0x26, 0x77, 0x6d, 0x68, 0x3c, 0xfb, 0xa3, 0x42, 0x71, 0xbc, 0x08, 0x79, 0x91, 0x3f, 
		0x5e, 0xf1, 0x3a, 0xed, 0x89, 0x6f, 0xd0, 0x6e, 0xd4, 0x22, 0x19, 0x95, 0x55, 0x86, 0xac, 0x56, 
		0x88, 0x27, 0x4e, 0x5c, 0x27, 0xf3, 0xe8, 0x6f, 0xf8, 0x63, 0x5e, 0xf6, 0xf4, 0x2d, 0xc8, 0x64, 
		0x9e, 0xc6, 0x76, 0x22, 0x15, 0xdc, 0xe1, 0x7a, 0xce, 0xd8, 0x99, 0x77, 0x31, 0x87, 0x6d, 0x9f, 
		0x1e, 0x57, 0x83, 0xe6, 0x1d, 0xcf, 0xc0, 0xca, 0x97, 0x3a, 0x8a, 0xb5, 0x38, 0x9b, 0x1b, 0xfe, 
		0x2d, 0xa0, 0xdd, 0xae, 0xe4, 0x81, 0x1c, 0x7d, 0xa1, 0xad, 0xc0, 0x71, 0xb9, 0x0d, 0x26, 0xd6, 
		0x18, 0xd0, 0xb8, 0x1c, 0x8b, 0x73, 0xf0, 0xd0, 0x8b, 0xdf, 0x7f, 0x90, 0xfb, 0xa3, 0x79, 0x5f, 
		0xe0, 0x42, 0xfd, 0xed, 0x60, 0x57, 0xd0, 0xde, 0x0f, 0xbb, 0xd4, 0xdd, 0xd4, 0xcb, 0x71, 0x1e, 
		0x62, 0x57, 0x64, 0x99, 0xd6, 0x5b, 0x65, 0x16, 0x6d, 0x40, 0x47, 0x64, 0x00, 0xea, 0xa4, 0xdf, 
		0x50, 0x6e, 0x78, 0xb8, 0xc2, 0xed, 0x69, 0x45, 0x1e, 0xdf, 0x99, 0x17, 0xea, 0xf8, 0xba, 0xb4, 
		0x42, 0xf0, 0x52, 0x3b, 0x21, 0x96, 0x90, 0x98, 0x2f, 0x0b, 0xc4, 0x4e, 0xd5, 0x97, 0xb3, 0xa9, 
		0xd9, 0x7f, 0x81, 0xf1, 0x5c, 0x22, 0x10, 0x6e, 0x18, 0xcb, 0x4b, 0xd1, 0x66, 0x0d, 0xc2, 0x14, 
		0xbd, 0x07, 0x21, 0x23, 0x57, 0x18, 0x87, 0xa4, 0xd4, 0xc5, 0x59, 0x32, 0xa4, 0xe7, 0x11, 0xce, 
		0xad, 0xe1, 0x63, 0x7a, 0xcf, 0xae, 0x76, 0xea, 0x1c, 0xd4, 0xa5, 0x51, 0x99, 0x89, 0x1d, 0x4f, 
		0x1f, 0x4d, 0x46, 0x88, 0xb3, 0xe8, 0xc4, 0x0e, 0xb5, 0xc5, 0x7e, 0x87, 0x66, 0x7a, 0xfd, 0x63, 
		0xc1, 0xd5, 0xfb, 0x5c, 0xd6, 0x09, 0x1f, 0xce, 0xda, 0x26, 0x8d, 0x1e, 0xb9, 0xfd, 0x35, 0xcd, 
		0x87, 0xb1, 0x37, 0x43, 0x8a, 0x91, 0x06, 0x2b, 0xee, 0xa8, 0x51, 0xe7, 0xa9, 0x86, 0x08, 0x3c, 
		0x1d, 0x70, 0xbb, 0x45, 0x2a, 0x81, 0x62, 0xb3, 0x59, 0x2e, 0x46, 0x07, 0x7d, 0xe1, 0x47, 0x75, 
		0xa7, 0x0d, 0x2f, 0xc4, 0xfe, 0xe4, 0x0d, 0x25, 0x5f, 0x5d, 0x71, 0x04, 0xd1, 0xc2, 0xcd, 0x57, 
		0xb0, 0x8b, 0xd7, 0x14, 0x0a, 0xb6, 0xea, 0x84, 0x33, 0x5e, 0xde, 0xeb, 0xa2, 0xc0, 0xe2, 0x37, 
		0x08, 0x4a, 0xcc, 0x3d, 0xcc, 0x33, 0x6f, 0xbe, 0x8a, 0xa9, 0xc3, 0xce, 0x85, 0x2b, 0x22, 0x73, 
		0xe8, 0x84, 0x89, 0xec, 0xcc, 0x10, 0xb9, 0x93, 0x57, 0xba, 0x60, 0xd2, 0x7a, 0xa7, 0x1e, 0x52, 
		0x43, 0x59, 0xd5, 0x23, 0xff, 0x65, 0x97, 0xf5, 0x4b, 0xe1, 0x3f, 0xff, 0xac, 0x90, 0xc8, 0x54, 
		0xbe, 0x49, 0x5e, 0x96, 0x95, 0xc5, 0x21, 0x04, 0x45, 0x2f, 0x30, 0x53, 0x57, 0xb8, 0x50, 0xee, 
		0xc8, 0xeb, 0x0e, 0x80, 0x39, 0xc5, 0xf3, 0xe3, 0x79, 0x76, 0x70, 0x60, 0xd8, 0x67, 0xd2, 0xa1, 
		0xb7, 0xb1, 0x37, 0xee, 0xa3, 0x67, 0xcb, 0x80, 0x07, 0xea, 0x83, 0xcc, 0xe2, 0x2b, 0x67, 0x84, 
		0x99, 0x88, 0x04, 0x34, 0x21, 0x36, 0xb2, 0x40, 0x09, 0x52, 0xf8, 0x12, 0xeb, 0xa9, 0x96, 0x57, 
		0x02, 0xd6, 0x4a, 0x15, 0x03, 0x42, 0x63, 0xb5, 0xa1, 0x5b, 0x65, 0xde, 0x69, 0xba, 0xe3, 0x11, 
		0x8e, 0x5a, 0x87, 0x9f, 0x5f, 0x97, 0x6c, 0x5f, 0x6d, 0x7e, 0x0b, 0x36, 0x4a, 0xd0, 0x7f, 0x74, 
		0xa1, 0x91, 0x9f, 0xbe, 0x5a, 0x80, 0x56, 0xa1, 0x06, 0xd9, 0xb3, 0xbe, 0xfc, 0xb2, 0xd9, 0x97, 
		0x44, 0xe0, 0xe8, 0x78, 0x31, 0xe2, 0x8b, 0xce, 0x6b, 0x8f, 0x71, 0x93, 0xf1, 0xff, 0xbd, 0xcc, 
		0xc8, 0xa7, 0x15, 0xc1, 0x50, 0x17, 0x7a, 0x2a, 0x36, 0xcf, 0x20, 0x76, 0x6f, 0xf8, 0x23, 0xec, 
		0x16, 0x5e, 0x9f, 0x8a, 0x28, 0x22, 0xbb, 0xb7, 0x72, 0x71, 0x85, 0x18, 0x5f, 0xa8, 0x50, 0x48, 
		0xff, 0xea, 0x0d, 0x3d, 0x0f, 0x96, 0x78, 0x08, 0x77, 0xab, 0xe2, 0x92, 0xeb, 0x70, 0x6b, 0xc9, 
		0xba, 0x1b, 0x87, 0xb1, 0xeb, 0x65, 0xc1, 0x13, 0xb1, 0x65, 0xeb, 0x61, 0x94, 0x85, 0x22, 0x13, 
		0xf4, 0xb6, 0xab, 0x4d, 0xa6, 0xa0, 0x39, 0x7a, 0xe4, 0x1d, 0x73, 0xf4, 0x2c, 0x15, 0x44, 0xbc, 
		0x75, 0xef, 0xc2, 0xa3, 0xf2, 0xd0, 0xee, 0xe1, 0x9b, 0xef, 0x43, 0xd0, 0xf8, 0xd3, 0xdc, 0xec, 
		0x1a, 0xa5, 0x2a, 0xfa, 0x0b, 0x57, 0x3f, 0xbe, 0xdb, 0xac, 0xe1, 0x5f, 0x42, 0xf7, 0x02, 0x58, 
		0xe4, 0x1d, 0x6f, 0x11, 0x4b, 0x9b, 0xb1, 0x25, 0xfa, 0x17, 0x08, 0xef, 0xd6, 0xe0, 0xad, 0xb5, 
		0xa2, 0x29, 0x55, 0x4b, 0x35, 0xf6, 0x4d, 0x9a, 0x7d, 0x33, 0xcf, 0xf0, 0x54, 0x75, 0xff, 0xb0, 
		0xb5, 0x25, 0x09, 0x8a, 0x89, 0xff, 0x03, 0xb3, 0x29, 0xf6, 0x85, 0x58, 0x7d, 0x13, 0x98, 0x6b, 
		0x67, 0x56, 0x8f, 0x0a, 0x9e, 0x1b, 0x7a, 0x26, 0x90, 0x1c, 0x73, 0xc1, 0xd7, 0xca, 0xe6, 0xe1, 
		0xeb, 0xcf, 0xd9, 0x78, 0xbf, 0x4d, 0xfa, 0xc7, 0xd7, 0x10, 0x5b, 0x6b, 0x3e, 0x21, 0x73, 0x44, 
		0x98, 0x97, 0xcd, 0xa9, 0x57, 0xf7, 0x42, 0x5f, 0x7f, 0xbd, 0x17, 0x76, 0x5b, 0x6c, 0x38, 0xd4, 
		0xcd, 0x45, 0x80, 0x20, 0x18, 0xe9, 0xe9, 0x70, 0xca, 0xf6, 0x7d, 0x99, 0x00, 0x1c, 0xea, 0xc4, 
		0x62, 0x98, 0x18, 0xac, 0xf4, 0xf1, 0x67, 0x15, 0xf7, 0xac, 0x9a, 0x80, 0x6f, 0x2c, 0xcb, 0xdf, 
		0x67, 0xc8, 0x20, 0x79, 0x10, 0x39, 0xab, 0xc1, 0x18, 0x42, 0x35, 0xac, 0x7e, 0xce, 0x7b, 0xab, 
		0x0e, 0x09, 0x8e, 0xdb, 0x0b, 0xee, 0x2a, 0x9e, 0x6b, 0x71, 0xee, 0xbc, 0x27, 0xab, 0x46, 0x26, 
		0x66, 0x19, 0xef, 0xb9, 0x3b, 0xb9, 0x6d, 0x50, 0xa0, 0x4f, 0xcf, 0xec, 0x86, 0x60, 0xf9, 0x54, 
		0x63, 0x2f, 0x5f, 0x42, 0x16, 0x00, 0x23, 0x21, 0x21, 0x69, 0x4a, 0x86, 0xac, 0x20, 0xd9, 0xd2, 
		0x4f, 0x3f, 0x4b, 0x42, 0xfd, 0x48, 0x31, 0x87, 0x7f, 0x4c, 0xda, 0x7b, 0x6c, 0x48, 0x8a, 0xb7, 
		0x7d, 0xe5, 0x02, 0x01, 0xf8, 0x05, 0xc0, 0x5d, 0x72, 0x18, 0xe6, 0x7e, 0xfb, 0xd7, 0xf1, 0x61, 
		0x28, 0x96, 0x12, 0x05, 0xd0, 0xda, 0x4d, 0xe7, 0x79, 0x3e, 0x7c, 0x8a, 0x84, 0x94, 0x5b, 0x67, 
		0x9f, 0x05, 0xee, 0x0d, 0x94, 0x26, 0x3e, 0x50, 0x8d, 0xd3, 0x22, 0x1c, 0xa4, 0x1c, 0x64, 0xe6, 
		0x8d, 0xcd, 0xe8, 0x86, 0x01, 0xe6, 0x6b, 0xb0, 0xa9, 0xed, 0xba, 0x61, 0x0a, 0xe3, 0xd8, 0xb6, 
		0x4e, 0x82, 0x73, 0xdd, 0x4f, 0x58, 0xb3, 0x10, 0xe4, 0x34, 0xf0, 0x55, 0x8b, 0x83, 0x5d, 0x35, 
		0x1c, 0xd5, 0x83, 0x24, 0xf2, 0xa8, 0x88, 0xb9, 0x28, 0xf4, 0x76, 0x9d, 0x9c, 0x5b, 0x06, 0x8b, 
		0x52, 0x60, 0xaf, 0x3e, 0xa7, 0x3c, 0x05, 0x85, 0xad, 0x29, 0x46, 0x48, 0x29, 0xf1, 0xc5, 0x39, 
		0xc8, 0x76, 0x2f, 0x38, 0x89, 0xae, 0x28, 0x7d, 0xa9, 0x93, 0x7b, 0x5a, 0x5e, 0xb2, 0x7a, 0xa4, 
		0xc9, 0x77, 0x9b, 0xde, 0x6a, 0xf3, 0xa2, 0xec, 0xf2, 0xf5, 0xf0, 0x38, 0x7c, 0x71, 0x21, 0xae, 
		0xa8, 0x9b, 0xcc, 0xf7, 0xae, 0x5e, 0x0e, 0x0c, 0x5d, 0xdf, 0xf0, 0x0d, 0xf5, 0xb7, 0x61, 0x3f, 
		0x62, 0x45, 0x7d, 0x46, 0x7d, 0xd1, 0xb8, 0x98, 0x4f, 0x9b, 0xba, 0x19, 0x83, 0x93, 0x01, 0x27, 
		0x19, 0x03, 0x4b, 0x27, 0xe1, 0x5d, 0x64, 0x37, 0xcd, 0xb0, 0x5b, 0x3d, 0x9c, 0xb9, 0x77, 0x6f, 
		0x24, 0x1b, 0x75, 0xf9, 0xc8, 0xc0, 0x5a, 0x19, 0xba, 0x78, 0x07, 0xa3, 0x21, 0xe6, 0x92, 0x4a, 
		0x37, 0xb1, 0xe7, 0xd2, 0x70, 0x27, 0x2a, 0xe6, 0xa6, 0xaf, 0x5b, 0xb5, 0x92, 0x9d, 0x11, 0x5e, 
		0xa8, 0xc0, 0xa2, 0xd6, 0x33, 0x8d, 0x73, 0x1b, 0x35, 0x38, 0x5e, 0x34, 0xa2, 0x24, 0x3f, 0x7e, 
		0xc6, 0x7b, 0x58, 0x46, 0x44, 0x8c, 0x1f, 0x53, 0xd8, 0x60, 0x50, 0xd3, 0x19, 0xdb, 0x45, 0x75, 
		0x5a, 0xb8, 0xeb, 0xcc, 0xc1, 0x1a, 0xdd, 0x1a, 0x9d, 0x9d, 0xc8, 0x3d, 0xd0, 0xd5, 0x79, 0xc4, 
		0xe0, 0x8e, 0x7e, 0xe4, 0x14, 0x6e, 0x9c, 0x3c, 0x95, 0x05, 0x1b, 0xcd, 0x57, 0x6f, 0xae, 0xb6, 
		0x6c, 0x73, 0x48, 0x91, 0x44, 0x4d, 0x46, 0x97, 0xde, 0x7f, 0x94, 0x74, 0xae, 0xc9, 0x53, 0x6c, 
		0xcc, 0x57, 0xcd, 0x34, 0xb0, 0xc7, 0x07, 0x65, 0xe8, 0xe6, 0x48, 0xd7, 0xd7, 0xea, 0xba, 0x73, 
		0x52, 0x72, 0xc7, 0x9b, 0x73, 0x1d, 0x44, 0xe5, 0x3c, 0x35, 0x25, 0xa0, 0x0c, 0xd1, 0x49, 0x4c, 
		0xab, 0x19, 0xbc, 0xcf, 0xe7, 0x5f, 0xe1, 0xe3, 0xea, 0xdd, 0x72, 0xfe, 0x28, 0x40, 0x2d, 0xd4, 
		0x57, 0xba, 0x09, 0x77, 0x1c, 0x69, 0xaf, 0xbe, 0xbc, 0xaf, 0x70, 0xb0, 0x68, 0x54, 0x96, 0x8d, 
		0x3f, 0x6c, 0x17, 0x14, 0x10, 0x59, 0xeb, 0x32, 0x0d, 0x2d, 0x38, 0x2f, 0x5c, 0x7a, 0x1c, 0x95, 
		0xd5, 0x13, 0x43, 0x55, 0x7d, 0x24, 0xb0, 0x2e, 0x28, 0xed, 0x00, 0xd6, 0xfb, 0x1b, 0x7b, 0x6f, 
		0xe0, 0xaa, 0x7b, 0xf4, 0x64, 0x95, 0xea, 0x20, 0xd4, 0x28, 0x11, 0xb1, 0x9b, 0xa4, 0x74, 0xb6, 
		0x00, 0x1a, 0xbf, 0xf9, 0x68, 0xf2, 0x5a, 0xc7, 0x1e, 0x78, 0x72, 0xf6, 0x96, 0x08, 0x3e, 0x70, 
		0x38, 0x01, 0x5e, 0xb7, 0xb2, 0xcd, 0x78, 0x80, 0x99, 0xa7, 0xf4, 0x48, 0x5c, 0x10, 0x72, 0xc9, 
		0x9a, 0x8f, 0xda, 0xec, 0x17, 0xfb, 0xff, 0x1c, 0x4f, 0xe6, 0xac, 0x97, 0x9f, 0xfa, 0x65, 0x28, 
		0x38, 0x77, 0x36, 0x25, 0x08, 0x0b, 0x83, 0xaa, 0x76, 0xc5, 0x9c, 0xc2, 0x31, 0x0f, 0x29, 0x38, 
		0x3f, 0x89, 0x85, 0x7a, 0xc4, 0x8e, 0x78, 0xc8, 0x43, 0xb7, 0x10, 0x6a, 0x3c, 0x9a, 0xdf, 0x00, 
		0x91, 0x50, 0xbe, 0xdd, 0x50, 0xa0, 0x4c, 0x45, 0x4c, 0xf3, 0xaa, 0xea, 0xac, 0x8c, 0x01, 0x6e, 
		0x1a, 0x60, 0x76, 0x81, 0x30, 0xcc, 0x6f, 0xb4, 0x13, 0x2f, 0x29, 0x9d, 0x6d, 0x5f, 0xbb, 0x36, 
		0xcc, 0xa8, 0x46, 0x12, 0x31, 0x5d, 0xea, 0x13, 0x1d, 0x74, 0x63, 0xb6, 0x62, 0x53, 0x65, 0xa8, 
		0xae, 0x40, 0x65, 0x39, 0xfc, 0x1b, 0x66, 0xb3, 0xae, 0x4f, 0x18, 0x69, 0xbf, 0x6a, 0x11, 0x6b, 
		0x01, 0xbd, 0xb1, 0xb9, 0xe7, 0xab, 0xc6, 0x16, 0x45, 0xd0, 0x8f, 0x3d, 0xce, 0xc4, 0x01, 0x8d, 
		0x3e, 0xac, 0xe1, 0x57, 0xe3, 0xb2, 0xaf, 0x6a, 0x14, 0x85, 0x7c, 0x83, 0x9a, 0x5e, 0xb6, 0xc6, 
		0xa2, 0xa3, 0x76, 0xdd, 0x97, 0x55, 0x9e, 0x63, 0x1c, 0x63, 0xba, 0xcb, 0x00, 0xc3, 0x1b, 0x93, 
		0x54, 0x67, 0xca, 0xe8, 0xdc, 0xf2, 0x23, 0x81, 0x1c, 0xfe, 0x4e, 0xfa, 0x23, 0x5f, 0x1a, 0x5f, 
		0xe2, 0xb1, 0xa0, 0xdc, 0x65, 0xbb, 0x04, 0xb6, 0x95, 0x2c, 0x05, 0xec, 0x48, 0xc9, 0x87, 0x37, 
		0xce, 0x0a, 0xd6, 0xdc, 0x3c, 0x3b, 0x5d, 0xa5, 0x62, 0x29, 0xd6, 0xbe, 0x98, 0x3a, 0xba, 0xe5, 
		0xb5, 0xc2, 0x4f, 0x17, 0xfa, 0xbf, 0x8e, 0x5e, 0xa2, 0x01, 0xd4, 0x56, 0x50, 0xe3, 0x33, 0xf8, 
		0x9a, 0x04, 0x8d, 0x78, 0x92, 0x22, 0xdc, 0x55, 0xc1, 0x60, 0xb5, 0x17, 0x1b, 0x3c, 0xaf, 0x28, 
		0x38, 0xf4, 0xb6, 0xc6, 0x14, 0xda, 0x15, 0xec, 0x8c, 0xf8, 0x8e, 0xcf, 0x4e, 0xa1, 0x1a, 0x59, 
		0x00, 0x03, 0x4e, 0x03, 0x88, 0x50, 0x77, 0x77, 0x6d, 0xbc, 0x8b, 0xcc, 0xcf, 0xb6, 0x9e, 0x11, 
		0x2a, 0x3a, 0x1c, 0x81, 0xc4, 0x96, 0x56, 0x09, 0xbf, 0xaf, 0x40, 0x31, 0x55, 0x26, 0xd2, 0xb1, 
		0x09, 0x0e, 0x79, 0x1b, 0x28, 0x52, 0x41, 0xc4, 0x2b, 0x99, 0xbe, 0x7a, 0xdc, 0xa1, 0x47, 0xc5, 
		0x8f, 0x2b, 0x5d, 0x39, 0xb1, 0x84, 0xe6, 0xae, 0x6b, 0x53, 0x5b, 0xaf, 0xb5, 0x7c, 0xb1, 0x3e, 
		0x80, 0xcc, 0xb4, 0x6e, 0x04, 0x4c, 0x6a, 0x79, 0x96, 0xba, 0x99, 0xd2, 0x00, 0xc5, 0x2a, 0x86, 
		0xad, 0x33, 0x18, 0x5d, 0xd3, 0x46, 0x17, 0xda, 0x84, 0x7a, 0x94, 0xf8, 0x54, 0x3f, 0x10, 0x0e, 
		0x56, 0x3c, 0xb8, 0xf2, 0xac, 0xe1, 0xaf, 0xd6, 0x0b, 0x75, 0x8c, 0xcb, 0xca, 0x34, 0xca, 0xe2, 
		0x1e, 0x7f, 0xf0, 0xb5, 0xb8, 0x14, 0x56, 0x91, 0xd2, 0xfc, 0xa0, 0x16, 0xfe, 0x41, 0x20, 0x35, 
		0xa2, 0xcf, 0x52, 0xde, 0x4b, 0xcb, 0x66, 0xa0, 0x2d, 0xa4, 0x07, 0x6c, 0xe0, 0x7c, 0x02, 0xf2, 
		0x44, 0x93, 0x5d, 0x1c, 0x73, 0x2a, 0xcf, 0xa7, 0x30, 0x74, 0x0f, 0xa2, 0x82, 0xd3, 0x62, 0xcb, 
		0x3d, 0x37, 0x64, 0xfe, 0xd9, 0xa7, 0xa5, 0xee, 0xc0, 0x65, 0x41, 0x14, 0x3a, 0x47, 0xfd, 0x1d, 
		0x3f, 0x47, 0xa9, 0xae, 0x17, 0xd6, 0xb3, 0x58, 0xd0, 0x62, 0x3f, 0x7e, 0xb1, 0xf0, 0xae, 0xbb, 
		0xc2, 0x0f, 0xe3, 0xc3, 0x69, 0x27, 0xd8, 0x3e, 0xb1, 0x29, 0x84, 0xa3, 0x64, 0x4d, 0x3c, 0x31, 
		0x56, 0x34, 0x75, 0x4f, 0x9e, 0xfc, 0x59, 0xb3, 0xf5, 0x0a, 0xe5, 0xca, 0xf3, 0x63, 0x2e, 0x55, 
		0x24, 0x84, 0xd8, 0x31, 0x5c, 0x81, 0xd0, 0x61, 0x2c, 0x87, 0x60, 0x4e, 0x70, 0x1d, 0x95, 0x03, 
		0xf7, 0x4a, 0x63, 0x74, 0xe6, 0x04, 0xfb, 0x4b, 0xb7, 0xf4, 0x3a, 0xbe, 0x81, 0x8d, 0xe2, 0x3d, 
		0xe6, 0x17, 0x60, 0x0f, 0xab, 0xae, 0x9f, 0xab, 0xa3, 0x68, 0xe4, 0xdf, 0xf1, 0x6a, 0x30, 0xa3, 
};

static int xcode_building_first_stage(struct cxdec_xcode_status *xcode)
{
  	switch (xcode_rand(xcode) % 3) {
  	case 0:
		// MOV ESI, EncryptionControlBlock
		// MOV EAX, DWORD PTR DS:[ESI+((xcode_rand(xcode) & 0x3ff) << 2)]
		if (!push_bytexcode(xcode, 0xbe)
				|| !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
				|| !push_2bytesxcode(xcode, 0x8b, 0x86)
				|| !push_dwordxcode(xcode, (xcode_rand(xcode) & 0x3ff) << 2))
			return 0;
		break;
	case 1:
  		// MOV EAX, EDI
  		if (!push_2bytesxcode(xcode, 0x8b, 0xc7))
  			return 0;
		break;
  	case 2:
		// MOV EAX, xcode_rand(xcode)
		if (!push_bytexcode(xcode, 0xb8)
				|| !push_dwordxcode(xcode, xcode_rand(xcode)))
			return 0;
  		break;
  	}
  	return 1;
}

static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage)
{
	if (stage == 1)
		return xcode_building_first_stage(xcode);

    if (xcode_rand(xcode) & 1) {
		if (!xcode_building_stage1(xcode, stage - 1))
			return 0;
    } else {
		if (!xcode_building_stage0(xcode, stage - 1))
			return 0;
    }

    switch (xcode_rand(xcode) & 7) {
	case 0:
		// NOT EAX
		if (!push_2bytesxcode(xcode, 0xf7, 0xd0))
			return 0;
        break;
	case 1:
		// MOV ESI, EncryptionControlBlock
		// AND EAX, 3FFh
		// MOV EAX, DWORD PTR DS:[ESI+EAX*4]
        if (!push_bytexcode(xcode, 0xbe)
  				|| !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
				|| !push_bytexcode(xcode, 0x25)
				|| !push_dwordxcode(xcode, 0x3ff)
				|| !push_3bytesxcode(xcode, 0x8b, 0x04, 0x86))
			return 0;
        break;
	case 2:
		// DEC EAX
		if (!push_bytexcode(xcode, 0x48))
			return 0;
        break;
	case 3:
		// NEG EAX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8))
			return 0;
		break;
	case 4:
        if (xcode_rand(xcode) & 1) {
        	// ADD EAX, xcode_rand(xcode)
			if (!push_bytexcode(xcode, 0x05))
            	return 0;
        } else {
        	// SUB EAX, xcode_rand(xcode)
			if (!push_bytexcode(xcode, 0x2d))
            	return 0;
        }
        if (!push_dwordxcode(xcode, xcode_rand(xcode)))
        	return 0;
        break;
	case 5:
		// PUSH EBX
		// MOV EBX, EAX
		// AND EBX, AAAAAAAA
		// AND EAX, 55555555
		// SHR EBX, 1
		// SHL EAX, 1
		// OR EAX, EBX
		// POP EBX
		if (!push_bytexcode(xcode, 0x53)
				|| !push_2bytesxcode(xcode, 0x89, 0xc3)
				|| !push_6bytesxcode(xcode, 0x81, 0xe3, 0xaa, 0xaa, 0xaa, 0xaa)
				|| !push_5bytesxcode(xcode, 0x25, 0x55, 0x55, 0x55, 0x55)
				|| !push_2bytesxcode(xcode, 0xd1, 0xeb)
				|| !push_2bytesxcode(xcode, 0xd1, 0xe0)
				|| !push_2bytesxcode(xcode, 0x09, 0xd8)
				|| !push_bytexcode(xcode, 0x5b))
			return 0;
		break;
	case 6:
		// INC EAX
		if (!push_bytexcode(xcode, 0x40))
			return 0;
		break;
	case 7:
		// XOR EAX, xcode_rand(xcode)
		if (!push_bytexcode(xcode, 0x35) 
				|| !push_dwordxcode(xcode, xcode_rand(xcode)))
			return 0;
        break;
	}
	return 1;
}

static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage)
{
	if (stage == 1)
		return xcode_building_first_stage(xcode);

	// PUSH EBX
	if (!push_bytexcode(xcode, 0x53))
		return 0;

	if (xcode_rand(xcode) & 1) {
		if (!xcode_building_stage1(xcode, stage - 1))
			return 0;
	} else {
		if (!xcode_building_stage0(xcode, stage - 1))
			return 0;
	}

	// MOV EBX, EAX
	if (!push_2bytesxcode(xcode, 0x89, 0xc3))
		return 0;

	if (xcode_rand(xcode) & 1) {
		if (!xcode_building_stage1(xcode, stage - 1))
			return 0;
	} else {
		if (!xcode_building_stage0(xcode, stage - 1))
			return 0;
	}

	switch (xcode_rand(xcode) % 6) {
	case 0:
		// ADD EAX, EBX
		if (!push_2bytesxcode(xcode, 0x01, 0xd8))
			return 0;
		break;
    case 1:
		// PUSH ECX
		// MOV ECX, EBX
		// AND ECX, 0F
		// SHR EAX, CL
		// POP ECX
		if (!push_bytexcode(xcode, 0x51)
				|| !push_2bytesxcode(xcode, 0x89, 0xd9)
				|| !push_3bytesxcode(xcode, 0x83, 0xe1, 0x0f)
				|| !push_2bytesxcode(xcode, 0xd3, 0xe8)
				|| !push_bytexcode(xcode, 0x59))
			return 0;
		break;
    case 2:
    	// PUSH ECX
    	// MOV ECX, EBX
    	// AND ECX, 0F
    	// SHL EAX, CL
    	// POP ECX
		if (!push_bytexcode(xcode, 0x51) 
				|| !push_2bytesxcode(xcode, 0x89, 0xd9)
				|| !push_3bytesxcode(xcode, 0x83, 0xe1, 0x0f)
				|| !push_2bytesxcode(xcode, 0xd3, 0xe0)
				|| !push_bytexcode(xcode, 0x59))
			return 0;
		break;
    case 3:
    	// NEG EAX, ADD EAX, EBX
		if (!push_2bytesxcode(xcode, 0xf7, 0xd8) 
				|| !push_2bytesxcode(xcode, 0x01, 0xd8))
			return 0;
		break;
    case 4:
    	// IMUL EAX, EBX
		if (!push_3bytesxcode(xcode, 0x0f,  0xaf, 0xc3))
			return 0;
		break;
    case 5:
    	// SUB EAX, EBX
		if (!push_2bytesxcode(xcode, 0x29, 0xd8))
			return 0;
		break;
	}
	// POP EBX
	return push_bytexcode(xcode, 0x5b);
}

struct cxdec_callback towa_cxdec_callback = {
	"towa",
	{ 0x3b5, 0x46b },
	xcode_building_stage1
};

struct cxdec_callback yomibito_cxdec_callback = {
	"yomibito",
	{ 0x3b5, 0x46b },
	xcode_building_stage1
};
