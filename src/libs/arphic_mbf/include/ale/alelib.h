/*

       Arphic Layout Engine    ver $Name$
           Copyright (C) 2001-2011 Arphic Technology Co., Ltd. All rights reserved.

         Additonal Library:
         include file for user

 */

#ifndef  __ARPHIC_ALELIB_H__
#define  __ARPHIC_ALELIB_H__


#include "aleuser.h"


#ifdef __cplusplus
extern "C"
 {
#endif



                  /*
                      Check if Unicode string Data [] can be fully displayed within screen's width
                      If it is too long, get the first line from Data [] to LineClipBuffer [].

                      Parameters:
                      Data [] (input): an Unicode string of the whole data to be displayed.
                         It may be very long

                      CodeType (input): please set to 1 to indicate this is an Unicode string

                      LineClipBuffer [] (output): an Unicode string buffer to get the clipped
                                string (the first line) if Data [] is too long.
                                It should be a little larger than the maximum number of
                                characters each line.

                      LineClipBufferSize (input): array size of LineClipBuffer []
                                          if you declare "ALE_UINT16 LineClipBuffer [100];"
                                          send LineClipBufferSize = 100.

                                          CAUTION: if you use flag ALE_LINECLIP_CUT_RESERVE_TAIL,
                                          you must set LineClipBufferSize =
                                            (100 - length of tail string).
                                          See the comments of flag ALE_LINECLIP_CUT_RESERVE_TAIL.

                      LineCharPos [] (input/output): a buffer for this function to calculate
                                          character position.
                                          Its size should be the same as LineClipBuffer [].

                                          Input:
                                          In general, you do not need to initialize this array
                                          when calling this API. However, if you use one
                                          of the following flag, you will need to set
                                          some element of LineCharPos [].
                                          Please see the remarks of these flags for more information.

                                          ALE_LINECLIP_REQUEST_CHARSPACE:
                                              LineCharPos [0] = character space in pixel

                                          ALE_LINECLIP_CROSS_LINE_WORD
                                              LineCharPos [2] = remain word length of previous line

                                          ALE_LINECLIP_CUT_RESERVE_TAIL
                                              LineCharPos [3] = width of tail string ("...") in pixels

                                          Output:
                                          If  LineClipBufferSize >= 2, LineCharPos [0]/[1]
                                          will be the  horizontal advance/outputbuffer width
                                          of the displayed string (probably clipped).
                                          If these value are negative, LineCharPos [0]/[1] will
                                          be set to zero.
                                          NOTE: if clipped line contain CR/LF,
                                          and Flag ALE_LINECLIP_SUPPORT_CRLF is set,
                                          CR/LF will not be included in above width.

                                          If you use flag
                                          ALE_LINECLIP_CROSS_LINE_WORD or
                                          ALE_LINECLIP_CUT_RESERVE_TAIL, more information
                                          will be returned in LineCharPos [2] & [3].
                                          Please see the remarks of these flags for more information.


                      WidthLimit (input): screen width limit in pixels

                      Flag (input):
                                    ALE_LINECLIP_CONTINUELASTLINE

                                         If you want call this function iteratively to output
                                         a long string in multiple lines, clear this flag
                                         to 0 when output the first line, keep returned
                                         LineClipBuffer [] unchanged and set this flag for
                                         outputting successive lines.
                                         When this flag is set, this
                                          function will try to get Unicode text directional
                                          control codes (such as LRO, PDF...) from
                                          LineClipBuffer [] (it should contain the result
                                          last time you call this function, i.e. the
                                          previous line) so effects of these Unicode
                                         control codes will not be stopped after a
                                         string is broken into several lines.
                                         See following code example.

                                    ALE_LINECLIP_ARABIC_CROSSLINE
                                         If this flag is set, AleLineClippingCheck () will
                                         assume that you will want Arabic ligatures to
                                         cross line. Please see following code example.

                                         (NOTE: if this flag is set, AleSetBiDiBorderJoinInArabic ()
                                          will be invoked, and this API will call
                                          AleSetBiDiBorderJoinInArabic (0xFFFF,0) before return.)

                                    ALE_LINECLIP_CUT_AT_SPACE
                                         If this flag is set, AleLineClippingCheck () will
                                         try to clip line at space if possible.

                                    ALE_LINECLIP_SUPPORT_CRLF
                                         If this flag is set, AleLineClippingCheck () will break line at
                                           CR(0x0D) or CR-LF (0x0D-0x0A)

                                    ALE_LINECLIP_ADVANCE_WORD_BREAK
                                         No matter whether render option ALE_RNDO_AdvanceCharGroupEnabled
                                         is turned on, try break line at Thai's(Myanmar's) word boundary.

                                    ALE_LINECLIP_REQUEST_CHARSPACE
                                         If you want space between characters, set this flag and set
                                         LineCharPos [0] to the space in pixel when calling this API

                                    ALE_LINECLIP_CROSS_LINE_WORD
                                         Request line clipping try to keep word longer than one line.
                                         Set LineCharPos [2] to the remain length of word not
                                         displayed at last line;
                                         when returned, LineCharPos [2] will be the remain length
                                         of last word if it is not totally displayed.
                                         WARNING:
                                          1.Only Thai and Myanmar are supported.
                                          2.this flag does not support bi-directional text.
                                           Using this flag for bi-directional text may result in unexpected error.

                                    ALE_LINECLIP_CUT_RESERVE_TAIL
                                         If the Data [] is clipped (not fully displayed), preserve some width when
                                         clipping the string. I.e. the clipped string will not use the full screen
                                         width. This is used if you want to add some "tail string" (ex: "...") after
                                         the clipped string to indicate that the string is clipped. By the preserved
                                         width the "..." can be displayed.

                                         To use this flag, you need to Set LineCharPos [3] = the width (in pixels)
                                         of the tail string (ex: "...").
                                         When returned, you can check the value of LineCharPos [3]. If it is not
                                         zero, this means the string is clipped, and you should add "..." to the
                                          end of the clipped string.

                                          CAUTION: in this case, you should be sure the real size of
                                          LineClipBuffer [] > LineClipBufferSize +  length (# of  characters) of "tail string".
                                          Therefore you can append the tail string to the end of LineClipBuffer [].
                                          OTHERWISE BUFFER OVERFLOW MAY OCCUR.


                                    ALE_LINECLIP_QUICK_CUT
                                         If text is longer than screen width, try to estimate the width of the
                                         cut string and the cut string reorderred  instead of calling
                                         AleGetStringFont () again. This will improve performance when
                                         given text needs to be cut.

                                         WARNING: "the result and reorderred string cut" may be different from
                                         "string cut and re-renderred" at last character, if last character is
                                           a. Arabic ligature connected with next character.
                                           b. between two characters different from current text direction.
                                              (ex: last character is a space between two Arabic characters
                                                in English-major text, or vice versa.)



                      WorkBuf (input): layout engine's working buffer

                      Return:
                         0: Data [] can be displayed within screen [], and its
                           string length < LineClipBufferSize.
                         => So, just display Data [] by AleGetStringFont ()

                       non-zero: string Data [] is longer than LineClipBufferSize
                         or screen width, so, this function has copied
                         the first line of Data [] is to LineClipBuffer [].
                         The return value is the string length extracted from
                           Data [] to LineClipBuffer []. (NOTE: If ContinueLastLine
                           is set to 1, LineClipBuffer [] may be longer then return value,
                           because some control code is duplicated for correct ouptut)
                         => So, just display LineClipBuffer [] by AleGetStringFont (),
                           and process from Data [return value] for next line.

                       Code Example:
                           Display Unicode string "icodestr []" in multiple lines,
                           screen width is 160 pixels,
                           at most 39 characters each line.
                           When Arabic is used, more code is added to keep
                           Arabic ligatures when crossing lines.

                           ALE_UINT16 line [40], charpos [40];
                           ALE_UINT16 screenwidth = 160;
                           ALE_INT32 kcut;
                           ALE_UINT16 *currline, *currdata;
                           ALE_INT32 ContLastLine;

                           ALE_INT32 prevtotal = 0;

                           currdata = icodestr;
                           ContLastLine = 0;

                           do
                            {
                               #ifdef __ARPHIC_LAYOUTENGINE_ARABIC__
                               AleSetBiDiBorderJoinInArabic
                                  ( (prevtotal > 0)? icodestr [prevtotal-1]: (ALE_UINT16) 0,
                                     0, AleWorkBuffer);
                               #endif

                               kcut = AleLineClippingCheck
                                   (currdata, 1, line, 40, charpos, screenwidth,
                                        ALE_LINECLIP_ARABIC_CROSSLINE |
                                     ((ContLastLine)? ALE_LINECLIP_CONTINUELASTLINE: 0),
                                     AleWorkBuffer);

                               if ( kcut > 0 )
                                  currline = line, currdata += kcut;
                               else
                                  currline = currdata;

                               #ifdef __ARPHIC_LAYOUTENGINE_ARABIC__
                                  AleSetBiDiBorderJoinInArabic (0xFFFF, (kcut > 0)? *currdata: (ALE_UINT16) 0, AleWorkBuffer);
                               #endif

                               < Display currline [] by AleGetStringFont () here.
                                 For the metrics of currline [],
                                 charpos [0] is horiAdvance, charpos [1] is outBufWidth>

                               ContLastLine = 1;
                               prevtotal += kcut;

                            } while (*currdata && kcut > 0 );

                  */

       ALE_INT32 AleLineClippingCheck (ALE_UINT16 *Data,
                    ALE_INT16 CodeType,
                    ALE_UINT16 *LineClipBuffer,  ALE_INT32 LineClipBufferSize,
                    ALE_UINT16 *LineCharPos,
                    ALE_UINT16 WidthLimit,
                    ALE_UINT32 Flag,
                    char *WorkBuf);

                   #define ALE_LINECLIP_CONTINUELASTLINE  1
                   #define ALE_LINECLIP_ARABIC_CROSSLINE  2
                   #define ALE_LINECLIP_CUT_AT_SPACE  4
                   #define ALE_LINECLIP_SUPPORT_CRLF    8
                   #define ALE_LINECLIP_ADVANCE_WORD_BREAK  0x10
                   #define ALE_LINECLIP_REQUEST_CHARSPACE 0x20
                   #define ALE_LINECLIP_CROSS_LINE_WORD 0x40
                   #define ALE_LINECLIP_CUT_RESERVE_TAIL 0x80
                   #define ALE_LINECLIP_QUICK_CUT 0x1000

                    /* for compatibility of old code */
                    #define ALE_LINECLIP_THAI_WORD_BREAK  ALE_LINECLIP_ADVANCE_WORD_BREAK





                    /*
                         A special version of AleLineClippingCheck (), which will add a
                         "tail string" (ex: "...") after the clipped string if string Data [] is
                         clipped.

                         TailString []: Unicode of the tail string (ended by zero)

                         TailStringWidth: width (in pixels) of the tail string. Set to zero
                           if you want this function to calculate it.
                         TailStringLen: length (in characters) of the tail string. Set to zero
                           if you want this function to calculate it.

                         Other parameters are the same as AleLineClippingCheck (),
                         except one thing: you should increase size of LineClipBuffer [],
                         LineCharPos [] and LineClipBufferSize by the length of tail string + 1.
                         If you set LineClipBufferSize = 100, and length of tail string is
                         3 (ex: "..."), the maximum returned string will have length < 100-3-1=96.

                         LineClipBufferSize must > tail string length, & must > 4.

                         Return value: the same as AleLineClippingCheck (), excpets that
                         you CANNOT not use the values returned in LineCharPos [].

                         WARNING: This function may add Unicode control code ZWJ
                         before your tail string and after the clipped string,  if the
                         string is clipped at the middle of an Arabic word. This is to
                         preserve the shape of Arabic characters.

                     */

       ALE_INT32 AleLineClippingCheckWithTailString (ALE_UINT16 *Data,
                    ALE_INT16 CodeType,
                    ALE_UINT16 *LineClipBuffer,  ALE_INT32 LineClipBufferSize,
                    ALE_UINT16 *LineCharPos,
                    ALE_UINT16 WidthLimit,
                    ALE_UINT32 Flag,

                    ALE_UINT16 *TailString,
                    ALE_INT32 TailStringWidth, ALE_INT32 TailStringLen,

                    char *WorkBuf);










                    #define ALE_BLOCKFLAG_RIGHTALIGN  1
                    #define ALE_BLOCKFLAG_CUT_AT_SPACE  2
                    #define ALE_BLOCKFLAG_SUPPORT_CRLF  4
                    #define ALE_BLOCKFLAG_ALWAYS_RENDER  8
                    #define ALE_BLOCKFLAG_BIDI_TAIL_REVISE  0x10
                    #define ALE_BLOCKFLAG_LINE_JUSTIFY   0x20
                    #define ALE_BLOCKFLAG_PARAGRAPH_JUSTIFY   0x60
                    #define ALE_BLOCKFLAG_CENTER_JUSTIFY   0x80
                    #define ALE_BLOCKFLAG_LAST_LINE_POS_ONLY  0x100
                    #define ALE_BLOCKFLAG_ADVANCE_WORD_BREAK  0x200
                    #define ALE_BLOCKFLAG_CROSS_LINE_WORD 0x400
                    #define ALE_BLOCKFLAG_DYNAMIC_LINE_HEIGHT 0x800
                    #define ALE_BLOCKFLAG_QUICK_CUT 0x1000
                    #define ALE_BLOCKFLAG_KEEP_RIGHT_SPACE 0x2000
                      /* WARNING: max flag is 0x8000 */

                    /* for compatibility of old code */
                    #define ALE_BLOCKFLAG_THAI_WORD_BREAK  ALE_BLOCKFLAG_ADVANCE_WORD_BREAK

                 typedef struct _AleGetStringFontInBlockInfo
                  {
                     ALE_UINT16  LineWidthLimit;
                                     /* maximum line display width in pixels.
                                         NOTE: it must
                                           if 0 is given, this API will calculate
                                           itself (by buffer's width and
                                           startx position)
                                      */

                     ALE_UINT16  LineLengthLimit; /* maximum line length (in characters) */

                     ALE_UINT16  LineTotalLimit;   /* at most how much lines (0 means unlimited) */
                     ALE_UINT16  TotalHeightLimit; /* limitation of total height (0 means unlimited) */

                     ALE_UINT16  LineHeight;       /* height of each line when display */

                     ALE_UINT16  Flag;
                                           /*
                                               OR combination of following flags

                                               ALE_BLOCKFLAG_RIGHTALIGN
                                                             display fonts right-aligned

                                               ALE_BLOCKFLAG_CUT_AT_SPACE
                                                             Try to break line at space

                                               ALE_BLOCKFLAG_SUPPORT_CRLF
                                                             Support CR/LF (U+000D, 000A) new line control code

                                               ALE_BLOCKFLAG_BIDI_TAIL_REVISE
                                                             When display a string in multiple lines, if the first
                                                             "strong type" character (i.e. characters which has
                                                             obvious text direction such as English or Arabic)
                                                             of next line is different from current TextDirection,
                                                             add a LRM/RLM mark at end of current line.
                                                             This is to avoid hyphen of English appears
                                                             at improper position when major text direction
                                                             is Arabic, and vice versa.

                                               ALE_BLOCKFLAG_LINE_JUSTIFY
                                                             When displaying lines, try to add space so the left
                                                             and right side of each line fills whole screen width.

                                               ALE_BLOCKFLAG_PARAGRAPH_JUSTIFY
                                                             As ALE_BLOCKFLAG_LINE_JUSTIFY, besides the last
                                                             line of each paragraph, or lines ended by newline.

                                               ALE_BLOCKFLAG_LAST_LINE_POS_ONLY
                                                             Only return character positions of the last displayed line.

                                                             Remark: Set this flag will also make LE's
                                                             LastRenderedStringLength and LastBiDiReorderBuffer
                                                             revised to remove control codes added during rendering,
                                                             so LE's cursor operation APIs will work properly.
                                                             This flag is generally used for designing editor applications.

                                               ALE_BLOCKFLAG_ADVANCE_WORD_BREAK
                                                    No matter whether render option ALE_RNDO_AdvanceCharGroupEnabled
                                                    is turned on, try break line at Thai's/Myanmar's word boundary.

                                               ALE_BLOCKFLAG_CROSS_LINE_WORD
                                                    Try to keep word longer than one line not
                                                    merged with following words.
                                                   WARNING:
                                                    1.Only Thai and Myanmar are supported.
                                                    2.this flag does not support bi-directional text.
                                                     Using this flag for bi-directional text may result in unexpected error.

                                               ALE_BLOCKFLAG_QUICK_CUT
                                                    Try to render quicker, especially if string contaion BiDirectional text,
                                                    such as Arabic or Hebrew.
                                                    NOTE: Enable this flag has the effect of
                                                        ALE_BLOCKFLAG_BIDI_TAIL_REVISE

                                                ALE_BLOCKFLAG_KEEP_RIGHT_SPACE
                                                    Reserve width of one character at right edge.
                                                    This is useful when you use ALE_BLOCKFLAG_RIGHTALIGN and
                                                    some character may extend beyond its horizontal advance
                                                    (ex: Italic font).
                                                    Keep this space can avoid characters being clipped at right edge.


                                            */

                     ALE_UINT16 *ClipBuffer;    /* buffer for clipping, its size must >= LineLengthLimit + 1 */
                     ALE_UINT16 *ClipPosBuffer;  /* buffer for clipping, its size must >= LineLengthLimit + 1 */

                     ALE_UINT16 *CharPositionY;
                                           /* If it is not NULL, it is a buffer to receive upper-left Y position
                                               of each character, its size must >= 1 + string length of
                                               the string to be displayed. */

                     ALE_UINT16 *CharDisplayWidth;
                                          /* If it is not NULL, it is a buffer to receive display
                                              width of each character
                                              its size must >= 1 + string length of
                                              the string to be displayed. */

                     ALE_UINT16 *OriginCharIndex;
                                          /* If it is not NULL, it is a buffer to receive the
                                              mapping from displayed string to origin string.
                                              If the first element is 5, it means the first displayed
                                              character is the 5th character of the given string.

                                              If you are using right-to-left text, this may be
                                              necessary, for the string will be reorderred before
                                              display.
                                              calling AleGetCharIndexInBlock () need this information.
                                              Its size must >= 1 + string length of
                                              the string to be displayed. */

                     ALE_UINT16 LineOutputedTotal;  /* # of lines outputted */
                     ALE_UINT16 CharOutputedTotal;  /* # of characters outputted */

                  } AleGetStringFontInBlockInfo;

       ALE_INT32 AleGetStringFontInBlock (AleGetStringFontInfo *AInfo,
                              AleGetStringFontInBlockInfo *ABlockInfo, char *WorkBuf);
                   /*
                       Display a string in a given buffer. If the string is too long,
                         this API will display it into multiple lines.
                    */

          ALE_INT32 AleGetCharIndexInBlock
                  (ALE_INT32 x, ALE_INT32 y, /* cursor position */
                   ALE_UINT16 *AString,    /* display string */
                   ALE_UINT16 *ACharPosition,
                                            /* X position array returned in
                                                  CharPosition [] of AleGetStringFontInfo
                                                  when calling AleGetStringFontInBlock () */

                   ALE_UINT16 *ACharPositionY, ALE_UINT16 *ACharPositionW,
                   ALE_UINT16 *AOriginCharIndex,
                                            /* Y/width array returned in
                                                CharPositionY [], CharDisplayWidth [],
                                                OriginCharIndex [] of
                                                AleGetStringFontInBlockInfo
                                                  when calling AleGetStringFontInBlock () */
                   ALE_INT16 ALineHeight,   /* line height */

                   ALE_INT32 *ADisplayIndex,
                   ALE_INT32 *ADisplayInfoIndex,
                                  /*if not NULL,
                                     ADisplayIndex will be set to the DISPLAY index of the character
                                     where cursor is on. 1 means the first character
                                     displayed. In general, it should be the same as
                                     return value; however, in right-to-left text,
                                     it is the DISPLAY order of the character where
                                     cursor is on (while the return value is the
                                     "DATA" order of the same character).

                                     Generally, ADisplayInfoIndex will be the same as
                                     ADisplayIndex. However, If the character is in Arabic ligature,
                                     ADisplayInfoIndex may be different from ADisplayIndex.
                                     You should use ACharPosition(Y,W) [ADisplayInfoIndex-1]
                                     instead of ACharPosition(Y,W) [ADisplayIndex-1] for
                                     position information.


                                     */

                   char *WorkBuf);

                     /*
                          Get the character where cursor( at (x,y)) is on
                           return: the index of the character, 1 means the
                             cursor is on the first character (i.e. AString [0]),
                             2 means the cursor is on the second character
                             (i.e. AString [1])... etc.
                             0 means cursor is not on any cursor.

                          NOTE: if you are using right-to-left text such as
                            Arabic, you may need to give ADisplayIndex
                            to receive the display order.

                          If return value is not zero,
                          AString [(return value) - 1]  is the character where
                          cursor is on, its upper-left corner is at
                           x = ACharPosition [*ADisplayIndex-1],
                           y = ACharPositionY [*ADisplayIndex-1],
                          and its width is  ACharPositionW [*ADisplayIndex-1],

                      */








          ALE_INT32 AleGetStringFontInRegion (AleGetStringFontInfo *AInfo,
                              AleGetStringFontInBlockInfo *ABlockInfo,
                              ALE_INT16 *ARegion,
                              ALE_INT16 ARegionPolygonTotal,
                              AleExtension *AExtension,
                              char *WorkBuf);
                      /*
                           Output Text to regions of multiple polygons.

                           AInfo, ABlockInfo: the same as in AleGetStringFontInBlock ()

                           ARegion: an ALE_INT16 array [] containing all polygons in the
                              following format:

                              [# of points of 1st Polygon] [(x,y) coordinates of 1st polygon]
                              [# of points of 2nd Polygon] [(x,y) coordinates of 2nd polygon]
                              ...
                              [# of points of last Polygon] [(x,y) coordinates of last polygon]
                              and end this array with a zero element.

                              Coordinates must be given in either clockwise or counterclockwise.

                              For example: to output text to a triangle and a rectangle:
                                triangle (x,y) = (300,30) (480,30) (480,150)
                                rectangle (x,y) = (500,30) (600,30) (600,100) (500,100)
                              Set ARegion to an array containing following 17 integers:
                              ALE_INT16 region [17] =
                                {3, 300, 30,  480, 30,  480, 150,
                                  4, 500, 30, 600, 30, 600, 100, 500, 100
                                   0};
                                   (never forget the ending zero, totally 17 integers)

                           ARegionPolygonTotal:
                                number of polygons in ARegion []

                           AExtension: generally, please set it to NULL.

                           Return value: the same as in AleGetStringFontInBlock ()

                           Remarks:
                             1. Every polygon must be "horizontal convex", that is,
                                 every horizontal line cannot have more than 2 intersections
                                 with the polygon borders. In the other words, no horizontal
                                 line inside polygon have sections outside the polygon.
                             2. Please be sure no the polygons will be out of range. (The range
                                 of the bitmap buffer you given, and the line total /height
                                 limits set in ABlockInfo)
                                 Otherwise, the result will be unpredictible.
                             3. If polygons are insufficient for all text given, the output
                                 will be terminated.

                       */







          #define AleGetStringFont_ThaiSimpleBlock AleGetStringFontInSimpleWordBlock

          ALE_INT32 AleGetStringFontInSimpleWordBlock (AleGetStringFontInfo *AInfo,
                              AleGetStringFontInBlockInfo *ABlockInfo, char *WorkBuf);
                                /*
                                     A simple version of AleGetStringFontInBlock () with
                                       reduced features but faster speed.
                                 */


          ALE_INT32 AleGetStringFontInSimpleThaiWordBlock (AleGetStringFontInfo *AInfo,
                              AleGetStringFontInBlockInfo *ABlockInfo,
                              ALE_INT16 *AThaiCharWidthCache, char *WorkBuf);
                                /*
                                     The same as AleGetStringFontInSimpleWordBlock (),
                                     but user can provide a cache of Thai character width
                                     to accelerate.
                                     (cache size should be 128, i.e. ALE_INT16 [128])

                                     NOTE: please initialize AThaiCharWidthCache [] by
                                       AleInitializeThaiCharWidthCache () at first use and
                                       whenever you start using a new font.

                                     This API will also initialize cache once width of U+0E12
                                     does not match AThaiCharWidthCache [0x12].
                                 */

                  void AleInitializeThaiCharWidthCache (ALE_INT16 *ACache, char *WorkBuf);




#ifdef __cplusplus
 };
#endif



#endif

