#include "ProcessorUtils.h"

#include "Core.h"
#include "Log.h"

namespace Cepums {

Instruction decodeInstruction(uint8_t opcode) {
    // Opcode is 6 bits, so 64 distinct values
    switch (opcode) {
        case 0x00: return Instruction::CallPal;

        case 0x08: return Instruction::LDA;
        case 0x09: return Instruction::LDAH;
        case 0x0A: return Instruction::LDBU;
        case 0x0B: return Instruction::LDQ_U;
        case 0x0C: return Instruction::LDWU;
        case 0x0D: return Instruction::STW;
        case 0x0E: return Instruction::STB;
        case 0x0F: return Instruction::STQ_U;

        case 0x10: return Instruction::INTA; // Uses function field, must be decoded further
        case 0x11: return Instruction::INTL; // Uses function field, must be decoded further
        case 0x12: return Instruction::INTS; // Uses function field, must be decoded further
        case 0x13: return Instruction::INTM; // Uses function field, must be decoded further
        case 0x14: return Instruction::ITFP; // Uses function field, must be decoded further
        case 0x15: return Instruction::FLTV; // Uses function field, must be decoded further
        case 0x16: return Instruction::FLTI; // Uses function field, must be decoded further
        case 0x17: return Instruction::FLTL; // Uses function field, must be decoded further

        case 0x18: return Instruction::MISC; // Uses function field, must be decoded further
        case 0x19: return Instruction::PAL2;
        case 0x1A: return Instruction::JSR;
        case 0x1B: return Instruction::HW_LD;
        case 0x1C: return Instruction::FPTI; // Uses function field, must be decoded further
        case 0x1D: return Instruction::MTPR; // Move To Processor Register (in PAL)
        case 0x1E: return Instruction::PAL4;
        case 0x1F: return Instruction::PAL5;

        case 0x20: return Instruction::LDF;
        case 0x21: return Instruction::LDG;
        case 0x22: return Instruction::LDS;
        case 0x23: return Instruction::LDT;
        case 0x24: return Instruction::STF;
        case 0x25: return Instruction::STG;
        case 0x26: return Instruction::STS;
        case 0x27: return Instruction::STT;

        case 0x28: return Instruction::LDL;
        case 0x29: return Instruction::LDQ;
        case 0x2A: return Instruction::LDL_L;
        case 0x2B: return Instruction::LDQ_L;
        case 0x2C: return Instruction::STL;
        case 0x2D: return Instruction::STQ;
        case 0x2E: return Instruction::STL_C;
        case 0x2F: return Instruction::STQ_C;

        case 0x30: return Instruction::BR; // Unconditional branch
        case 0x31: return Instruction::FBEQ;
        case 0x32: return Instruction::FBLT;
        case 0x33: return Instruction::FBLE;
        case 0x34: return Instruction::BSR;
        case 0x35: return Instruction::FBNE;
        case 0x36: return Instruction::FBGE;
        case 0x37: return Instruction::FBGT;

        case 0x38: return Instruction::BLBC;
        case 0x39: return Instruction::BEQ;
        case 0x3A: return Instruction::BLT;
        case 0x3B: return Instruction::BLE;
        case 0x3C: return Instruction::BLBS;
        case 0x3D: return Instruction::BNE;
        case 0x3E: return Instruction::BGE;
        case 0x3F: return Instruction::BGT;

        default: UNKNOWN_INSTRUCTION();
    }
    return Instruction::Invalid;
}

InstructionFormat instructionFormat(Instruction instr) {
    switch (instr) {
        case Instruction::LDA:
        case Instruction::LDAH:
        case Instruction::LDBU:
        case Instruction::LDQ_U:
        case Instruction::LDWU:
        case Instruction::STW:
        case Instruction::STB:
        case Instruction::STQ_U:
        case Instruction::MISC:
        case Instruction::JSR:
        case Instruction::LDF:
        case Instruction::LDG:
        case Instruction::LDS:
        case Instruction::LDT:
        case Instruction::STF:
        case Instruction::STG:
        case Instruction::STS:
        case Instruction::STT:
        case Instruction::LDL:
        case Instruction::LDQ:
        case Instruction::LDL_L:
        case Instruction::LDQ_L:
        case Instruction::STL:
        case Instruction::STQ:
        case Instruction::STL_C:
        case Instruction::STQ_C: return InstructionFormat::Memory;

        case Instruction::BR:
        case Instruction::FBEQ:
        case Instruction::FBLT:
        case Instruction::FBLE:
        case Instruction::BSR:
        case Instruction::FBNE:
        case Instruction::FBGE:
        case Instruction::FBGT:
        case Instruction::BLBC:
        case Instruction::BEQ:
        case Instruction::BLT:
        case Instruction::BLE:
        case Instruction::BLBS:
        case Instruction::BNE:
        case Instruction::BGE:
        case Instruction::BGT: return InstructionFormat::Branch;

        case Instruction::INTA:
        case Instruction::INTL:
        case Instruction::INTS:
        case Instruction::INTM:
        case Instruction::FPTI: // Unsure about this one
        case Instruction::ITFP: return InstructionFormat::Operate;

        case Instruction::FLTV:
        case Instruction::FLTI:
        case Instruction::FLTL: return InstructionFormat::FloatPointOperate;

        // CallPal
        case Instruction::CallPal:
        case Instruction::PAL2:
        case Instruction::HW_LD: // Aka PAL1B
        case Instruction::MTPR: // I think I can keep this here?
        case Instruction::PAL4:
        case Instruction::PAL5: return InstructionFormat::PALcode;

        default: ILLEGAL_INSTRUCTION();
    }

    return InstructionFormat::Invalid;
}

Instruction decodeFunctionedInstruction(Instruction instr, uint16_t function) {
    switch (instr) {
        case Instruction::INTA: // 0x10
            switch (function) {
                case 0x00: return Instruction::ADDL;
                case 0x02: return Instruction::S4ADDL;
                case 0x09: return Instruction::SUBL;
                case 0x12: return Instruction::S8ADDL;
                case 0x1B: return Instruction::S8SUBQ;
                case 0x20: return Instruction::ADDQ;
                case 0x22: return Instruction::S4ADDQ;
                case 0x29: return Instruction::SUBQ;
                case 0x32: return Instruction::S8ADDQ;
                case 0x40: return Instruction::ADDL_V; // ???
                case 0x49: return Instruction::SUBL_V; // ???
                case 0x60: return Instruction::ADDQ_V; // ???
                case 0x0B: return Instruction::S4SUBL;
                case 0x0F: return Instruction::CMPBGE;
                case 0x1D: return Instruction::CMPULT;
                case 0x2B: return Instruction::S4SUBQ;
                case 0x2D: return Instruction::CMPEQ;
                case 0x3B: return Instruction::S8SUBQ;
                case 0x3D: return Instruction::CMPULE;
                case 0x4D: return Instruction::CMPLT;
                case 0x5D: return Instruction::CMPLE;
                case 0x69: return Instruction::SUBQ_V; // ???

                default: ILLEGAL_INSTRUCTION(); return Instruction::Invalid;
            }
            return Instruction::Invalid;

        case Instruction::INTL: // 0x11
            switch (function) {
                case 0x00: return Instruction::AND;
                case 0x08: return Instruction::BIC;
                case 0x14: return Instruction::CMOVLBS;
                case 0x16: return Instruction::CMOVLBC;
                case 0x20: return Instruction::BIS;
                case 0x24: return Instruction::CMOVEQ;
                case 0x26: return Instruction::CMOVNE;
                case 0x28: return Instruction::ORNOT;

                case 0x40: return Instruction::XOR;
                case 0x44: return Instruction::CMOVLT;
                case 0x46: return Instruction::CMOVGE;
                case 0x48: return Instruction::EQV;
                case 0x64: return Instruction::CMOVLE;
                case 0x66: return Instruction::CMOVGT;
                case 0x6C: return Instruction::IMPLVER;

                default: ILLEGAL_INSTRUCTION(); return Instruction::Invalid;
            }
            return Instruction::Invalid;

        case Instruction::INTS: // 0x12
            switch (function) {
                case 0x02: return Instruction::MSKBL;
                case 0x06: return Instruction::EXTBL;
                case 0x0B: return Instruction::INSBL;
                case 0x12: return Instruction::MSKWL;
                case 0x16: return Instruction::EXTWL;
                case 0x1B: return Instruction::INSWL;
                case 0x22: return Instruction::MSKLL;
                case 0x26: return Instruction::EXTLL;
                case 0x2B: return Instruction::INSLL;
                case 0x30: return Instruction::ZAP;
                case 0x31: return Instruction::ZAPNOT;
                case 0x32: return Instruction::MSKQL;
                case 0x34: return Instruction::SRL;
                case 0x36: return Instruction::EXTQL;
                case 0x39: return Instruction::SLL;
                case 0x3B: return Instruction::INSQL;
                case 0x3C: return Instruction::SRA;
                case 0x52: return Instruction::MSKWH;
                case 0x57: return Instruction::INSWH;
                case 0x5A: return Instruction::EXTWH;
                case 0x62: return Instruction::MSKLH;
                case 0x67: return Instruction::INSLH;
                case 0x6A: return Instruction::EXTLH;
                case 0x72: return Instruction::MSKQH;
                case 0x77: return Instruction::INSQH;
                case 0x7A: return Instruction::EXTQH;

                default: ILLEGAL_INSTRUCTION(); return Instruction::Invalid;
            }
            return Instruction::Invalid;

        case Instruction::INTM: // 0x13
            switch (function) {
                case 0x00: return Instruction::MULL;
                case 0x20: return Instruction::MULQ;
                case 0x30: return Instruction::UMULH;
                case 0x40: return Instruction::MULL_V;
                case 0x60: return Instruction::MULQ_V;

                default: ILLEGAL_INSTRUCTION(); return Instruction::Invalid;
            }
            return Instruction::Invalid;

        case Instruction::ITFP: // 0x14
            switch (function) {
                case 0x004: return Instruction::ITOFS;
                case 0x014: return Instruction::ITOFF;
                case 0x024: return Instruction::ITOFT;
                case 0x08A: return Instruction::SQRTF;
                case 0x08B: return Instruction::SQRTS;
                case 0x0AA: return Instruction::SQRTG;
                case 0x0AB: return Instruction::SQRTT;

                default: ILLEGAL_INSTRUCTION(); return Instruction::Invalid;
            }
            return Instruction::Invalid;

        case Instruction::FLTV: // 0x15
            switch (function) {
                case 0x080: return Instruction::ADDF;
                case 0x081: return Instruction::SUBF;
                case 0x082: return Instruction::MULF;
                case 0x083: return Instruction::DIVF;
                case 0x09E: return Instruction::CVTDG;
                case 0x0A0: return Instruction::ADDG;
                case 0x0A1: return Instruction::SUBG;
                case 0x0A2: return Instruction::MULG;
                case 0x0A3: return Instruction::DIVG;
                case 0x0A5: return Instruction::CMPGEQ;
                case 0x0A6: return Instruction::CMPGLT;
                case 0x0A7: return Instruction::CMPGLE;
                case 0x0AC: return Instruction::CVTGF;
                case 0x0AD: return Instruction::CVTGD;
                case 0x0AF: return Instruction::CVTGQ;
                case 0x0BC: return Instruction::CVTQF;
                case 0x0BE: return Instruction::CVTQG;

                default: ILLEGAL_INSTRUCTION(); return Instruction::Invalid;
            }
            return Instruction::Invalid;

        case Instruction::FLTI: // 0x16
            switch (function) {
                case 0x080: return Instruction::ADDS;
                case 0x081: return Instruction::SUBS;
                case 0x082: return Instruction::MULS;
                case 0x083: return Instruction::DIVS;
                case 0x0A0: return Instruction::ADDT;
                case 0x0A1: return Instruction::SUBT;
                case 0x0A2: return Instruction::MULT;
                case 0x0A3: return Instruction::DIVT;
                case 0x0A4: return Instruction::CMPTUN;
                case 0x0A5: return Instruction::CMPTEQ;
                case 0x0A6: return Instruction::CMPTLT;
                case 0x0A7: return Instruction::CMPTLE;
                case 0x0AC: return Instruction::CVTTS;
                case 0x0AF: return Instruction::CVTTQ;
                case 0x0BC: return Instruction::CVTQS;
                case 0x0BE: return Instruction::CVTQT;
                case 0x2AC: return Instruction::CVTST;

                default: ILLEGAL_INSTRUCTION(); return Instruction::Invalid;
            }
            return Instruction::Invalid;

        case Instruction::FLTL: // 0x17
            switch (function) {
                case 0x010: return Instruction::CVTLQ;
                case 0x020: return Instruction::CPYS;
                case 0x021: return Instruction::CPYSN;
                case 0x022: return Instruction::CPYSE;
                case 0x024: return Instruction::MT_FPCR;
                case 0x025: return Instruction::MF_FPCR;
                case 0x02A: return Instruction::FCMOVEQ;
                case 0x02B: return Instruction::FCMOVNE;
                case 0x02C: return Instruction::FCMOVLT;
                case 0x02D: return Instruction::FCMOVGE;
                case 0x02E: return Instruction::FCMOVLE;
                case 0x02F: return Instruction::FCMOVGT;
                case 0x030: return Instruction::CVTQL;

                default: ILLEGAL_INSTRUCTION(); return Instruction::Invalid;
            }
            return Instruction::Invalid;
        case Instruction::MISC: // 0x18
            switch (function) {
                case 0x0000: return Instruction::TRAPB;
                case 0x4000: return Instruction::MB;
                case 0x4400: return Instruction::WMB;
                case 0x8000: return Instruction::FETCH;
                case 0xA000: return Instruction::FETCH_M;
                case 0xC000: return Instruction::RPCC;
                case 0xE000: return Instruction::RC;
                case 0xF000: return Instruction::RS;
                case 0xF800: return Instruction::WH64;
                case 0xFC00: return Instruction::WH64EN;

                default: ILLEGAL_INSTRUCTION(); return Instruction::Invalid;
            }
            return Instruction::Invalid;
        case Instruction::FPTI: // 0x1C
            switch (function) {
                case 0x00: return Instruction::SEXTB;
                case 0x01: return Instruction::SEXTW;
                case 0x30: return Instruction::CTPOP;
                case 0x31: return Instruction::PERR;
                case 0x32: return Instruction::CTLZ;
                case 0x33: return Instruction::CTTZ;
                case 0x34: return Instruction::UNPKBW;
                case 0x35: return Instruction::UNPKBL;
                case 0x36: return Instruction::PKWB;
                case 0x37: return Instruction::PKLB;
                case 0x38: return Instruction::MINSB8;
                case 0x39: return Instruction::MINSW4;
                case 0x3A: return Instruction::MINUB8;
                case 0x3B: return Instruction::MINUW4;
                case 0x3C: return Instruction::MAXUB8;
                case 0x3D: return Instruction::MAXUW4;
                case 0x3E: return Instruction::MAXSB8;
                case 0x3F: return Instruction::MAXSW4;
                case 0x70: return Instruction::FTOIT;
                case 0x78: return Instruction::FTOIS;

                default: ILLEGAL_INSTRUCTION(); return Instruction::Invalid;
            }
            return Instruction::Invalid;

        default: ILLEGAL_INSTRUCTION();
    }

    return Instruction::Invalid;
}

std::string instructionMnemonic(Instruction instr) {
    switch (instr) {
        case Instruction::Invalid: return "Invalid";
        case Instruction::CallPal: return "CallPal";
        case Instruction::LDA: return "lda";
        case Instruction::LDAH: return "ldah";
        case Instruction::BR: return "br";
        case Instruction::LDBU: return "ldbu";
        case Instruction::LDQ_U: return "lsq_u";
        case Instruction::LDWU: return "ldwu";
        case Instruction::STW: return "stw";
        case Instruction::STB: return "stb";
        case Instruction::STQ_U: return "stq_u";
        case Instruction::INTA: return "inta";
        case Instruction::ADDL: return "addl";
        case Instruction::S4ADDL: return "s4addl";
        case Instruction::SUBL: return "subl";
        case Instruction::S8ADDL: return "s8addl";
        case Instruction::S8SUBQ: return "s8subq";
        case Instruction::ADDQ: return "addq";
        case Instruction::S4ADDQ: return "s4addq";
        case Instruction::SUBQ: return "subq";
        case Instruction::S8ADDQ: return "s8addq";
        case Instruction::ADDL_V: return "addl_v";
        case Instruction::SUBL_V: return "subl_v";
        case Instruction::ADDQ_V: return "addq_v";
        case Instruction::S4SUBL: return "s4subl";
        case Instruction::CMPBGE: return "cmpbge";
        case Instruction::CMPULT: return "cmpult";
        case Instruction::S4SUBQ: return "s4subq";
        case Instruction::CMPEQ: return "cmpeq";
        case Instruction::CMPULE: return "cmpule";
        case Instruction::CMPLT: return "cmplt";
        case Instruction::CMPLE: return "cmple";
        case Instruction::SUBQ_V: return "subq_v";
        case Instruction::INTL: return "intl";
        case Instruction::AND: return "and";
        case Instruction::BIC: return "bic";
        case Instruction::CMOVLBS: return "cmovlbs";
        case Instruction::CMOVLBC: return "cmovlbc";
        case Instruction::BIS: return "bis";
        case Instruction::CMOVEQ: return "cmoveq";
        case Instruction::CMOVNE: return "cmovne";
        case Instruction::ORNOT: return "ornot";
        case Instruction::XOR: return "xor";
        case Instruction::CMOVLT: return "cmovlt";
        case Instruction::CMOVGE: return "cmovge";
        case Instruction::EQV: return "eqv";
        case Instruction::CMOVLE: return "cmovle";
        case Instruction::CMOVGT: return "cmovgt";
        case Instruction::IMPLVER: return "implver";
        case Instruction::INTS: return "ints";
        case Instruction::MSKBL: return "mskbl";
        case Instruction::EXTBL: return "extbl";
        case Instruction::INSBL: return "insbl";
        case Instruction::MSKWL: return "mskwl";
        case Instruction::EXTWL: return "extwl";
        case Instruction::INSWL: return "inswl";
        case Instruction::MSKLL: return "mskll";
        case Instruction::EXTLL: return "extll";
        case Instruction::INSLL: return "insll";
        case Instruction::ZAP: return "zap";
        case Instruction::ZAPNOT: return "zapnot";
        case Instruction::MSKQL: return "mskl";
        case Instruction::SRL: return "srl";
        case Instruction::EXTQL: return "extql";
        case Instruction::SLL: return "sll";
        case Instruction::INSQL: return "insql";
        case Instruction::SRA: return "sra";
        case Instruction::MSKWH: return "mskwh";
        case Instruction::INSWH: return "inswh";
        case Instruction::EXTWH: return "extwh";
        case Instruction::MSKLH: return "msklh";
        case Instruction::INSLH: return "inslh";
        case Instruction::EXTLH: return "extlh";
        case Instruction::MSKQH: return "mskqh";
        case Instruction::INSQH: return "insqh";
        case Instruction::EXTQH: return "extqh";
        case Instruction::INTM: return "intm";
        case Instruction::MULL: return "mull";
        case Instruction::MULQ: return "mulq";
        case Instruction::UMULH: return "umulh";
        case Instruction::MULL_V: return "mull_v";
        case Instruction::MULQ_V: return "mulq_v";
        case Instruction::ITFP: return "itfp";
        case Instruction::ITOFS: return "itofs";
        case Instruction::ITOFF: return "itoff";
        case Instruction::ITOFT: return "itoft";
        case Instruction::SQRTF: return "sqrtf";
        case Instruction::SQRTS: return "sqrts";
        case Instruction::SQRTG: return "sqrtg";
        case Instruction::SQRTT: return "sqrtt";
        case Instruction::FLTV: return "fltv";
        case Instruction::ADDF: return "addf";
        case Instruction::SUBF: return "subf";
        case Instruction::MULF: return "mulf";
        case Instruction::DIVF: return "divf";
        case Instruction::CVTDG: return "cvtdg";
        case Instruction::ADDG: return "addh";
        case Instruction::SUBG: return "subg";
        case Instruction::MULG: return "mulg";
        case Instruction::DIVG: return "divg";
        case Instruction::CMPGEQ: return "cmpgeq";
        case Instruction::CMPGLT: return "cmpglt";
        case Instruction::CMPGLE: return "cmpgle";
        case Instruction::CVTGF: return "cvtgf";
        case Instruction::CVTGD: return "cvtgd";
        case Instruction::CVTGQ: return "cvtgq";
        case Instruction::CVTQF: return "cvtqf";
        case Instruction::CVTQG: return "cvtqg";
        case Instruction::FLTI: return "flti";
        case Instruction::ADDS: return "adds";
        case Instruction::SUBS: return "subs";
        case Instruction::MULS: return "muls";
        case Instruction::DIVS: return "divs";
        case Instruction::ADDT: return "addt";
        case Instruction::SUBT: return "subt";
        case Instruction::MULT: return "mult";
        case Instruction::DIVT: return "divt";
        case Instruction::CMPTUN: return "cmptun";
        case Instruction::CMPTEQ: return "cmpteq";
        case Instruction::CMPTLT: return "cmptlt";
        case Instruction::CMPTLE: return "cmptle";
        case Instruction::CVTTS: return "cvtts";
        case Instruction::CVTTQ: return "cvttq";
        case Instruction::CVTQS: return "cvtqs";
        case Instruction::CVTQT: return "cvtqt";
        case Instruction::CVTST: return "cvtst";
        case Instruction::FLTL: return "fltl";
        case Instruction::CVTLQ: return "cvtlq";
        case Instruction::CPYS: return "cpys";
        case Instruction::CPYSN: return "cpysn";
        case Instruction::CPYSE: return "cpyse";
        case Instruction::MT_FPCR: return "mt_fpcr";
        case Instruction::MF_FPCR: return "mf_fpcr";
        case Instruction::FCMOVEQ: return "fcmoveq";
        case Instruction::FCMOVNE: return "fcmovne";
        case Instruction::FCMOVLT: return "fcmovlt";
        case Instruction::FCMOVGE: return "fcmovge";
        case Instruction::FCMOVLE: return "fcmovle";
        case Instruction::FCMOVGT: return "fcmovgt";
        case Instruction::CVTQL: return "cvtql";
        case Instruction::MISC: return "misc";
        case Instruction::TRAPB: return "trapb";
        case Instruction::MB: return "mb";
        case Instruction::WMB: return "wmb";
        case Instruction::FETCH: return "fetch";
        case Instruction::FETCH_M: return "fetch_m";
        case Instruction::RPCC: return "rpcc";
        case Instruction::RC: return "rc";
        case Instruction::RS: return "rs";
        case Instruction::WH64: return "wh64";
        case Instruction::WH64EN: return "wh64en";
        case Instruction::PAL2: return "pal2";
        case Instruction::JSR: return "jsr3";
        case Instruction::HW_LD: return "hw_ld"; // PAL1B
        case Instruction::FPTI: return "fpti";
        case Instruction::SEXTB: return "sextb";
        case Instruction::SEXTW: return "sextw";
        case Instruction::CTPOP: return "ctpop";
        case Instruction::PERR: return "perr";
        case Instruction::CTLZ: return "ctlz";
        case Instruction::CTTZ: return "cttz";
        case Instruction::UNPKBW: return "unpkbw";
        case Instruction::UNPKBL: return "unpkbl";
        case Instruction::PKWB: return "pkwb";
        case Instruction::PKLB: return "pklb";
        case Instruction::MINSB8: return "minsb8";
        case Instruction::MINSW4: return "minsw4";
        case Instruction::MINUB8: return "minusb8";
        case Instruction::MINUW4: return "minuw";
        case Instruction::MAXUB8: return "maxub8";
        case Instruction::MAXUW4: return "maxuw4";
        case Instruction::MAXSB8: return "maxsb8";
        case Instruction::MAXSW4: return "maxsw4";
        case Instruction::FTOIT: return "ftoit";
        case Instruction::FTOIS: return "ftois";
        case Instruction::MTPR: return "mtpr";
        case Instruction::PAL4: return "pal4";
        case Instruction::PAL5: return "pal5";
        case Instruction::LDF: return "ldf";
        case Instruction::LDG: return "ldg";
        case Instruction::LDS: return "lds";
        case Instruction::LDT: return "ldt";
        case Instruction::STF: return "stf";
        case Instruction::STG: return "stg";
        case Instruction::STS: return "sts";
        case Instruction::STT: return "stt";
        case Instruction::LDL: return "ldl";
        case Instruction::LDQ: return "ldq";
        case Instruction::LDL_L: return "ldl_l";
        case Instruction::LDQ_L: return "ldq_l";
        case Instruction::STL: return "stl";
        case Instruction::STQ: return "stq";
        case Instruction::STL_C: return "stl_c";
        case Instruction::STQ_C: return "stq_c";
        case Instruction::FBEQ: return "fbeq";
        case Instruction::FBLT: return "fblt";
        case Instruction::FBLE: return "fble";
        case Instruction::BSR: return "bsr";
        case Instruction::FBNE: return "fbne";
        case Instruction::FBGE: return "fbge";
        case Instruction::FBGT: return "fbgt";
        case Instruction::BLBC: return "blbc";
        case Instruction::BEQ: return "beq";
        case Instruction::BLT: return "blt";
        case Instruction::BLE: return "ble";
        case Instruction::BLBS: return "blbs";
        case Instruction::BNE: return "bne";
        case Instruction::BGE: return "bge";
        case Instruction::BGT: return "bgt";
        default: VERIFY_NOT_REACHED(); return "";
    }
}

}; // namespace Cepums