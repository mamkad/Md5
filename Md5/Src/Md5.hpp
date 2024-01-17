#pragma once

#include <string>
#include <array>
#include <vector>

using std::string;
using std::array;
using std::vector;

namespace Lib
{
    namespace Algs
    {
        namespace Hash 
        {
            class Md5 final 
            {
                using TCode = uint32_t;

                enum Const
                {
                    BuffSize = 4,
                    ShiftSize = 64,
                    BlockInBits = 512,
                    Step = BlockInBits / 8
                };

                struct Context
                {
                    vector<TCode>          IntArray;
                    size_t                 LengthInBits = 0U;
                    array<TCode, BuffSize> HashBuff;
                };

                static array<TCode, BuffSize>  Buffs;
                static array<TCode, ShiftSize> Consts;
                static array<TCode, ShiftSize> Shifts;

            public:
                Md5()                      = delete;
                Md5(Md5 const&)            = delete;
                Md5& operator=(Md5 const&) = delete;

                static string hash(string const& str);

            private:
                static void   setup(Context& context);
                static void   convertStringToIntArray(string const& str, Context& context);
                static void   addZerosToIntArray(Context& context);
                static void   addLengthToEndOfIntArray(Context& context);
                static void   getHash(Context& context);
                static string convertHashToString(Context& context);
            };
        }
    }
}