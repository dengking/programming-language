
   inline bool twoway_bitwise_interleave(const unsigned char* begin1, const unsigned char* end1,
                                         const unsigned char* begin2, const unsigned char* end2,
                                         unsigned char* out)
   {
      if (std::distance(begin1,end1) != std::distance(begin2,end2))
      {
         return false;
      }

      static const std::size_t interleave_table_size = 256;
      static const unsigned short interleave_table[interleave_table_size] =
                                  {
                                     0x0000, 0x0001, 0x0004, 0x0005, 0x0010, 0x0011, 0x0014, 0x0015, // 0x00 - 0x07
                                     0x0040, 0x0041, 0x0044, 0x0045, 0x0050, 0x0051, 0x0054, 0x0055, // 0x08 - 0x0F
                                     0x0100, 0x0101, 0x0104, 0x0105, 0x0110, 0x0111, 0x0114, 0x0115, // 0x10 - 0x17
                                     0x0140, 0x0141, 0x0144, 0x0145, 0x0150, 0x0151, 0x0154, 0x0155, // 0x18 - 0x1F
                                     0x0400, 0x0401, 0x0404, 0x0405, 0x0410, 0x0411, 0x0414, 0x0415, // 0x20 - 0x27
                                     0x0440, 0x0441, 0x0444, 0x0445, 0x0450, 0x0451, 0x0454, 0x0455, // 0x28 - 0x2F
                                     0x0500, 0x0501, 0x0504, 0x0505, 0x0510, 0x0511, 0x0514, 0x0515, // 0x30 - 0x37
                                     0x0540, 0x0541, 0x0544, 0x0545, 0x0550, 0x0551, 0x0554, 0x0555, // 0x38 - 0x3F
                                     0x1000, 0x1001, 0x1004, 0x1005, 0x1010, 0x1011, 0x1014, 0x1015, // 0x40 - 0x47
                                     0x1040, 0x1041, 0x1044, 0x1045, 0x1050, 0x1051, 0x1054, 0x1055, // 0x48 - 0x4F
                                     0x1100, 0x1101, 0x1104, 0x1105, 0x1110, 0x1111, 0x1114, 0x1115, // 0x50 - 0x57
                                     0x1140, 0x1141, 0x1144, 0x1145, 0x1150, 0x1151, 0x1154, 0x1155, // 0x58 - 0x5F
                                     0x1400, 0x1401, 0x1404, 0x1405, 0x1410, 0x1411, 0x1414, 0x1415, // 0x60 - 0x67
                                     0x1440, 0x1441, 0x1444, 0x1445, 0x1450, 0x1451, 0x1454, 0x1455, // 0x68 - 0x6F
                                     0x1500, 0x1501, 0x1504, 0x1505, 0x1510, 0x1511, 0x1514, 0x1515, // 0x70 - 0x77
                                     0x1540, 0x1541, 0x1544, 0x1545, 0x1550, 0x1551, 0x1554, 0x1555, // 0x78 - 0x7F
                                     0x4000, 0x4001, 0x4004, 0x4005, 0x4010, 0x4011, 0x4014, 0x4015, // 0x80 - 0x87
                                     0x4040, 0x4041, 0x4044, 0x4045, 0x4050, 0x4051, 0x4054, 0x4055, // 0x88 - 0x8F
                                     0x4100, 0x4101, 0x4104, 0x4105, 0x4110, 0x4111, 0x4114, 0x4115, // 0x90 - 0x97
                                     0x4140, 0x4141, 0x4144, 0x4145, 0x4150, 0x4151, 0x4154, 0x4155, // 0x98 - 0x9F
                                     0x4400, 0x4401, 0x4404, 0x4405, 0x4410, 0x4411, 0x4414, 0x4415, // 0xA0 - 0xA7
                                     0x4440, 0x4441, 0x4444, 0x4445, 0x4450, 0x4451, 0x4454, 0x4455, // 0xA8 - 0xAF
                                     0x4500, 0x4501, 0x4504, 0x4505, 0x4510, 0x4511, 0x4514, 0x4515, // 0xB0 - 0xB7
                                     0x4540, 0x4541, 0x4544, 0x4545, 0x4550, 0x4551, 0x4554, 0x4555, // 0xB8 - 0xBF
                                     0x5000, 0x5001, 0x5004, 0x5005, 0x5010, 0x5011, 0x5014, 0x5015, // 0xC0 - 0xC7
                                     0x5040, 0x5041, 0x5044, 0x5045, 0x5050, 0x5051, 0x5054, 0x5055, // 0xC8 - 0xCF
                                     0x5100, 0x5101, 0x5104, 0x5105, 0x5110, 0x5111, 0x5114, 0x5115, // 0xD0 - 0xD7
                                     0x5140, 0x5141, 0x5144, 0x5145, 0x5150, 0x5151, 0x5154, 0x5155, // 0xD8 - 0xDF
                                     0x5400, 0x5401, 0x5404, 0x5405, 0x5410, 0x5411, 0x5414, 0x5415, // 0xE0 - 0xE7
                                     0x5440, 0x5441, 0x5444, 0x5445, 0x5450, 0x5451, 0x5454, 0x5455, // 0xE8 - 0xEF
                                     0x5500, 0x5501, 0x5504, 0x5505, 0x5510, 0x5511, 0x5514, 0x5515, // 0xF0 - 0xF7
                                     0x5540, 0x5541, 0x5544, 0x5545, 0x5550, 0x5551, 0x5554, 0x5555  // 0xF8 - 0xFF
                                  };

      const unsigned char* itr1 = begin1;
      const unsigned char* itr2 = begin2;

      while (end1 != itr1)
      {
         *(reinterpret_cast<unsigned short*>(out))  = (interleave_table[*(itr2++)] << 1);
         *(reinterpret_cast<unsigned short*>(out)) |=  interleave_table[*(itr1++)];
         out += 2;
      }

      return true;
   }

   inline bool twoway_bitwise_interleave(const char* begin1, const char* end1,
                                         const char* begin2, const char* end2,
                                         char* out)
   {
      return twoway_bitwise_interleave(reinterpret_cast<const unsigned char*>(begin1),
                                       reinterpret_cast<const unsigned char*>(end1),
                                       reinterpret_cast<const unsigned char*>(begin2),
                                       reinterpret_cast<const unsigned char*>(end2),
                                       reinterpret_cast<unsigned char*>(out));
   }

   inline bool twoway_bitwise_interleave(const std::string& str1,
                                         const std::string& str2,
                                         std::string& out)
   {
      if (str1.size() != str2.size())
      {
         return false;
      }

      out.resize(str1.size());

      return twoway_bitwise_interleave(to_ptr(str1), to_ptr(str1) + str1.size(),
                                       to_ptr(str2), to_ptr(str2) + str2.size(),
                                       const_cast<char*>(to_ptr(out)));
   }

   template <std::size_t n>
   struct interleave_ary;

   template<> struct interleave_ary<sizeof(unsigned short)>         { typedef unsigned short         type; };
   template<> struct interleave_ary<sizeof(unsigned int  )>         { typedef unsigned int           type; };
   template<> struct interleave_ary<sizeof(unsigned long long int)> { typedef unsigned long long int type; };

   template <std::size_t n>
   inline void create_nway_interleave_table(typename interleave_ary<n>::type table[256])
   {
      typedef typename interleave_ary<n>::type type;

      const type diff = static_cast<type>(n - 1);

      for (type i = static_cast<type>(0); i < static_cast<type>(256); ++i)
      {
         table[i] = 0x00;

         for (type j = static_cast<type>(0); j < static_cast<type>(8); ++j)
         {
            table[i] |= (i & (1 << j)) << (j * diff);
         }
      }
   }

   namespace bitwise_operation { enum type { eAND, eOR, eXOR }; }

   inline void bitwise_transform(const bitwise_operation::type& operation,
                                 const unsigned char* begin1, const unsigned char* end1,
                                 const unsigned char* begin2,
                                 unsigned char* out)
   {
      const unsigned char* itr1 = begin1;
      const unsigned char* itr2 = begin2;

      switch (operation)
      {
         case bitwise_operation::eAND : while (itr1 != end1) { *(out++) = *(itr1++) & *(itr2++); } return;
         case bitwise_operation::eOR  : while (itr1 != end1) { *(out++) = *(itr1++) | *(itr2++); } return;
         case bitwise_operation::eXOR : while (itr1 != end1) { *(out++) = *(itr1++) ^ *(itr2++); } return;
      }
   }

   inline void bitwise_transform(const bitwise_operation::type& operation,
                                 const char* begin1, const char* end1,
                                 const char* begin2,
                                 char* out)
   {
      bitwise_transform(operation,
                        reinterpret_cast<const unsigned char*>(begin1),
                        reinterpret_cast<const unsigned char*>(end1),
                        reinterpret_cast<const unsigned char*>(begin2),
                        reinterpret_cast<unsigned char*>(out));
   }

   inline void bitwise_transform(const bitwise_operation::type& operation,
                                 const std::string& str1,
                                 const std::string& str2,
                                 std::string& out)
   {
      if (str1.size() != str2.size()) return;
      out.resize(str1.size());
      bitwise_transform(operation,
                        to_ptr(str1), to_ptr(str1) + str1.size(),
                        to_ptr(str2),
                        const_cast<char*>(to_ptr(out)));
   }

   inline std::size_t high_bit_count(const unsigned char c)
   {
      static const std::size_t high_bits_in_char[256] =
                               {
                                  0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,
                                  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                                  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                                  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                                  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                                  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                                  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                                  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                                  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                                  4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8
                               };

      return high_bits_in_char[c];
   }

   inline std::size_t high_bit_count(const unsigned short& s)
   {
      const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&s);
      return high_bit_count(*(ptr + 0)) + high_bit_count(*(ptr + 1));
   }

   inline std::size_t high_bit_count(const unsigned int& i)
   {
      const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&i);
      return high_bit_count(*(ptr + 0)) + high_bit_count(*(ptr + 1)) +
             high_bit_count(*(ptr + 2)) + high_bit_count(*(ptr + 3));
   }

   inline std::size_t high_bit_count(const long long int& ll)
   {
      const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&ll);
      return high_bit_count(*(ptr + 0)) + high_bit_count(*(ptr + 1)) +
             high_bit_count(*(ptr + 2)) + high_bit_count(*(ptr + 3)) +
             high_bit_count(*(ptr + 4)) + high_bit_count(*(ptr + 5)) +
             high_bit_count(*(ptr + 6)) + high_bit_count(*(ptr + 7));
   }

   inline std::size_t high_bit_count(const unsigned char* begin, const unsigned char* end)
   {
      std::size_t count = 0;
      const unsigned char* itr = begin;

      while (end != itr)
      {
         count += high_bit_count(*itr++);
      }

      return count;
   }

   inline std::size_t high_bit_count(const char* begin, const char* end)
   {
      return high_bit_count(reinterpret_cast<const unsigned char*>(begin),
                            reinterpret_cast<const unsigned char*>(end));

   }

   inline std::size_t high_bit_count(const std::string& str)
   {
      return high_bit_count(to_ptr(str), to_ptr(str) + str.size());
   }

   inline bool bit_state(const std::size_t& index, const unsigned char* ptr)
   {
      static const unsigned char bit_mask[] =
                                 {
                                    0x01, //00000001
                                    0x02, //00000010
                                    0x04, //00000100
                                    0x08, //00001000
                                    0x10, //00010000
                                    0x20, //00100000
                                    0x40, //01000000
                                    0x80  //10000000
                                 };
      return (0 != (ptr[(index >> 3)] & bit_mask[index & 7]));
   }

   inline void set_bit_high(const std::size_t& index, unsigned char* const ptr)
   {
      static const unsigned char bit_mask[] =
                                 {
                                    0x01, //00000001
                                    0x02, //00000010
                                    0x04, //00000100
                                    0x08, //00001000
                                    0x10, //00010000
                                    0x20, //00100000
                                    0x40, //01000000
                                    0x80  //10000000
                                 };
      ptr[(index >> 3)] |= bit_mask[index & 7];
   }

   inline void set_bit_low(const std::size_t& index, unsigned char* const ptr)
   {
      static const unsigned char bit_mask[] =
                                 {
                                    0xFE, //11111110
                                    0xFD, //11111101
                                    0xFB, //11111011
                                    0xF7, //11110111
                                    0xEF, //11101111
                                    0xDF, //11011111
                                    0xBF, //10111111
                                    0x7F  //01111111
                                 };
      ptr[(index >> 3)] &= bit_mask[index & 7];
   }
