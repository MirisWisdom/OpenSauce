/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;

//TXboxAdpcmDecoder  0.1.3
//by Luigi Auriemma
//e-mail: aluigi@autistici.org
//web:    aluigi.org

// based on code from the TXboxAdpcmDecoder class of Benjamin Haisch (Revision 2 with stereo support)


namespace YeloDebug
{
    class XboxAudioStream
    {
        const uint kXboxAdpcmSrcSize = 36;
        const uint kXboxAdpcmDstSize = 130;

        static readonly short[] kStepTable = 
        {
            7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
            19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
            50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
            130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
            337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
            876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
            2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
            5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
            15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
        };

        static readonly sbyte[] kIndexTable = 
        {
            -1, -1, -1, -1, 2, 4, 6, 8,
            -1, -1, -1, -1, 2, 4, 6, 8
        };

		struct AdpcmState
		{
			public sbyte Index;
			public short StepSize;
			public short Predictor;
		};

        static int Delimit(int value, int high, int low)
        {
            if (value > high) return high;
            else if (value < 1) return low;
            else return value;
        }

        static int DecodeSample(int code, ref AdpcmState state)
        {
            int Delta, Result;
            Delta = state.StepSize >> 3;
            if ((code & 4) > 0) Delta += state.StepSize;
            if ((code & 2) > 0) Delta += state.StepSize >> 1;
            if ((code & 1) > 0) Delta += state.StepSize >> 2;
            if ((code & 8) > 0) Delta = -Delta;
            Result = state.Predictor + Delta;
            Result = Delimit(Result, short.MaxValue, short.MinValue);
            state.Index += kIndexTable[code];
            state.Index = (sbyte)Delimit(state.Index, 88, 0);
            state.StepSize = kStepTable[state.Index];
            state.Predictor = (short)Result;
            return Result;
        }

        static uint DecodeMemory(ushort[] input, uint length, ushort[] output, int channels)
        {
            AdpcmState[] adpcmState = new AdpcmState[channels];
            short[][] buffers = new short[][] { new short[channels], new short[8] };
            uint codeBuffer;
            int i, j, c, outLength;

            int inIndex = 0;
            int outIndex = 0;

            length = (uint)((length / kXboxAdpcmSrcSize) / channels);

            for (outLength = 0; length-- > 0; outLength++)
            {
                for (c = 0; c < channels; c++)
                {
                    output[outIndex] = input[0];
                    outIndex++;
                    output[outIndex] = input[1];
                    outIndex++;

                    adpcmState[c].Predictor = (short)(input[0] | (input[1] << 8));
                    inIndex += 2;
                    //adpcmState[c].Index = (sbyte)(input[0] | (input[1] << 8));  // doesnt make sense, the last one will always be 0
                    adpcmState[c].Index = (sbyte)input[0];
                    inIndex += 2;

                    adpcmState[c].Index = (sbyte)Delimit(adpcmState[c].Index, 88, 0);
                }

                for (i = 0; i < 8; i++)
                {
                    for (c = 0; c < channels; c++)
                    {
                        codeBuffer = (uint)(input[0] | (input[1] << 8) | (input[2] << 16) | (input[3] << 24));
                        inIndex += 4;
                        for (j = 0; j < 8; j++)
                        {
                            buffers[c][j] = (short)DecodeSample((int)codeBuffer & 15, ref adpcmState[c]);
                            codeBuffer >>= 4;   // divide by 16
                        }
                    }
                }

                for (j = 0; j < 8; j++)
                {
                    for (c = 0; c < channels; c++)
                    {
                        output[outIndex] = (ushort)(buffers[c][j] & 0xFF);
                        outIndex++;
                        output[outIndex] = (ushort)((buffers[c][j] >> 8) & 0xFF);
                        outIndex++;
                    }
                }
            }

            return (uint)(outLength * kXboxAdpcmDstSize * channels);
        }

        static uint GuessOutputSize(uint sourceSize)
        {
            return (uint)((sourceSize / kXboxAdpcmSrcSize) * kXboxAdpcmDstSize);
        }

        //public uint GetDirectSoundPlayAddress()
        //{
        //    // assumes everything we need is in header...why would they put it elsewhere???
        //    try
        //    {
        //        // buffer to store our xbe header
        //        byte[] xbeh = GetMemory(0x10000, (uint)0x1000);
        //        BinaryReader header = new BinaryReader(new System.IO.MemoryStream(xbeh));

        //        header.BaseStream.Position = 0x11C;
        //        uint SegmentCount = header.ReadUInt32();                //gets segment count
        //        uint SegmentBase = header.ReadUInt32() - 0x10000;       //gets base address of segment info table

        //        //loops through each segment
        //        for (int i = 0; i < SegmentCount; i++)
        //        {
        //            header.BaseStream.Position = (uint)(SegmentBase + i * 56) + 4;
        //            uint SegAddress = header.ReadUInt32();
        //            uint SegSize = header.ReadUInt32();
        //            header.BaseStream.Position += 8;
        //            header.BaseStream.Position = header.ReadUInt32() - 0x10000;
        //            string SegName = ASCIIEncoding.ASCII.GetString(header.ReadBytes(3));

        //            if (SegName.Equals("DSO"))
        //            {
        //                //dumps xpp segment
        //                byte[] SegDump = GetMemory(SegAddress, SegSize);

        //                //searches for xinputgetstate function
        //                for (int j = 0; j < SegSize; j++)
        //                    if ((BitConverter.ToUInt64(SegDump, j) & 0xFFFFFFFFFFFF) == 0x000004461246F766)
        //                        return (uint)(SegAddress + j);

        //                int asdf = 0;
        //            }
        //        }
        //    }
        //    catch
        //    {
        //        return 0;
        //    }
        //    throw new Exception("Unable to find XInputGetState() in memory, you must manually specify this address instead if you wish to initialize a controller hook.");
        //}
    }
}