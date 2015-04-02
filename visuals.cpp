#include "visuals.h"

#ifdef move_order_stats
//prints the move stats:
/*
 Number of possible moves: [x]
 ---------------------------------------------------
 [move number]: [how often this  was best move] [how often best move in percent (100% = move_order[x][0]+...+move_order[x][MAX_NUM_])] [(#ifdef cuttof_stats) how often a cutoff ocurret after this move]
 ...
 
 ...
 
 */
void printMoveOrderStats(){
    std::cout << "Move Order Stats" << std::endl << std::endl;
    int total_number_of_moves;
    for(int possible_moves = 0; possible_moves < MAX_NUMBER_OF_MOVES; ++possible_moves){
        total_number_of_moves = 0;
        for(int move_number = 0; move_number <= possible_moves; ++move_number){
            total_number_of_moves += Globals::Log::move_order[possible_moves][move_number];
        }
        std::cout << "---------------------------------------------------------" << std::endl;
        std::cout << possible_moves << " possible moves have " << total_number_of_moves << " best moves" << std::endl;
        std::cout << "---------------------------------------------------------" << std::endl;
        for(int move_number = 0; move_number <= possible_moves; ++move_number){
            std::cout << "Move number: " << move_number << " -> " << Globals::Log::move_order[possible_moves][move_number];
            if(total_number_of_moves == 0){
                std::cout << " = undefined %" << std::endl;
            }else{
                float tnm = total_number_of_moves;
                float percent = (100/tnm);
                percent *= Globals::Log::move_order[possible_moves][move_number];
                std::cout << " = " << percent << "%" << std::endl;
            }
//#ifdef move_cutoff_state
//            std::cout << " cutoff count: " << Globals::Log::move_cutoff[possible_moves][move_number];
//#endif
//            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}
#endif

void printSortedMoveList(Move* move, int length)
{
    printf("\n\n\n=========SortedMoveList:=========\n");
    for(int i = 0; i < length; i++){
        std::cout << i << "  " << move[i].from << "        ||      " << move[i].to << "             val:      " << move[i].value << "  ||\n";
    }
    printf("==============End================\n\n\n");
}

void theBreadfish()
{
    printf("=====================BREADFISH==================\n\n");
    printf("                        `,::`                   \n");
    printf("  ;`             `'':::::::::::'.               \n");
    printf("   ;#'     ,  ;;:::::::::::::::::::;;           \n");
    printf("    ;##'   ';::::::::::::::::::::::::::;'       \n");
    printf("    `######;;::::::::::::::::::::::::::::;;''   \n");
    printf("    ;#######;:::::::::::::::::::::'':,``  `` :' \n");
    printf("  .##;: '####'';::::::::::::::;;, ``````````` : \n");
    printf("`.        :###;.,:';:::::::::;````,,;;;;',```,: \n");
    printf("          ::;#;.....,:';::::'.``.,,,###';;;'`,` \n");
    printf("             `;.........,:';',``,,,;######;;':` \n");
    printf("              ;.............,:':,,,'##'####';;` \n");
    printf("              ;...............':,,,;#########;' \n");
    printf("              '...............':,,,,#########;' \n");
    printf("                ,'............+:,,,,#######'''' \n");
    printf("                    .',.......+:,,,,,..#####';` \n");
    printf("                        `':...+:,,,,`,:':       \n");
    printf("                             '':;';`            \n\n\n");
    printf("  Tell me, have you seen the marvelous Breadfish?\n");
    printf("================================================\n\n\n");
        
}

void theShark()
{
    printf("\n\n=====(B)EATEN-BREADFISH-(BY-SHARK)=============\n");
    printf("                            ```                \n");
    printf("                         `,:,,:.               \n");
    printf("          ``.```..``   `:....:`                \n");
    printf("         ```.`.`.``,.`.,....;.`                \n");
    printf("       `.``€€.`€€```.`.....`:.``               \n");
    printf("    `.::.```..`````...........:,.``    `.:::,  \n");
    printf("  `;,.:::::::::::.`.::..`..... ..;,.` .:..:.   \n");
    printf(" ,:::::::::::::::::,\\-\\............:.:,..:.`   \n");
    printf(" .,.@.:::::::::::.\\\\  \\.............,...:``    \n");
    printf("   ,.``.:::.`::::\\\\`  \\\\....#';.....`....,,`   \n");
    printf("     ,,:::;::\\\\`    \\\\,...+,:::::::.:...,,`    \n");
    printf("         `,..\\\\     \\\\,::\\:::::::::`   `:;:`   \n");
    printf("                   \\,`,.`.\\......:,            \n");
    printf("                    `,,,,.`\\.....:,.`          \n");
    printf("                             `:,,,::           \n");
    printf("                                ,;,            \n");
    printf("                                 `             \n");
    printf("  ...a vic for fisherman and sharks.     \n");
    printf("===================================================\n\n\n");
}

void theVictory()
{
    printf("\n\n=====================================================\n");
    printf("QQQQQQQQQQQQQQQQQQQQP???)???4QQQQQQQQQQQQQQQQQQQQQ\n");
    printf("QQQQQQQQQQQQQQQQQQ'            4QQQQQQQQQQQQQQQQQQ\n");
    printf("QQQQQQQQQQQQQQQQQ           P'  )QQQQQQQQQQQQQQQQQ\n");
    printf("QQQQQQQQQQQQQQQQf  _/    _aa/     QQQQQQQQQQQQQQQQ\n");
    printf("QQQQQQQQQQQQQQQQf_P 4f  ]Q  Qf    QQQQQQQQQQQQQQQQ\n");
    printf("QQQQQQQQQQQQQQQQf)   6  ]f  4Q    4QQQQQQQQQQQQQQQ\n");
    printf("QQQQQQQQQQQQQQQQf ]  QQQQQ  /'    ]QQQQQQQQQQQQQQQ\n");
    printf("QQQQQQQQQQQQQQQQf)QQQQQQQQQP4P    )WQQQQQQQQQQQQQQ\n");
    printf("QQQQQQQQQQQQQQQQf   \\???/aQP4f  6a ]QQQQQQQQQQQQQQ\n");
    printf("QQQQQQQQQQQQQQQQ'_6  ---'_yQQQ/     )QQQQQQQQQQQQQ\n");
    printf("QQQQQQQQQQQQQQP  jQQa   QQQQQQQf     )QQQQQQQQQQQQ\n");
    printf("QQQQQQQQQQQQQf  jQQQQQ QQQQQQQQQ/      4QQQQQQQQQQ\n");
    printf("QQQQQQQQQQQP'   QQQQQQQQQQQQQQQQ6  )f   )WQQQQQQQQ\n");
    printf("QQQQQQQQQQ    jQQQQQ       QQQQQQQ6   \\    4QQQQQQ\n");
    printf("QQQQQQQQQ'   ]QQQQ  VICTORY  QQQQQQf  ]f   )QQQQQQ\n");
    printf("QQQQQQQ' ]f ]QQQQQQQ       QQQQQQQQf   f    ]QQQQQ\n");
    printf("QQQQQQ6a    QQQQQQQQQQQQQQQQQQQQQQQf        ]QQQQQ\n");
    printf("QQQQQQQQ QQ  )WQQQQQQQQQQQQQQQQQQ'j6        QQQQQQ\n");
    printf("QQQQP' QQQQQ6  )4QQQQQQQQQQQQQQQQ]QQQ/  aj##f QQQQ\n");
    printf(" Q  QQQQQQQQQQ/   4QQQQQQQQQQQQQQ'QQQQQQQ###6   QQ\n");
    printf("f QQQQQQQQQQQQQ6   jQQQQQQQQQQQP4 ]QQQQQQQQ###aa 4\n");
    printf(" QQQQQQQQQQQQQQQ6]QQQQQQQQQQQP' ] ]QQQQQQQQQQ###Q'\n");
    printf("' QQQQQQQQQQQQQQQf)6 )????'    _P jQQQQQQQ### ?_yQ\n");
    printf("a    QQQQQQQQQQQQP ]'          )f QQQQ###P _ QQQQQ\n");
    printf("QQQQQQ6 QQQQ4QQQP _ajQQQQQQQQQQQf  ?##P' jQQQQQQQQ\n");
    printf("QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ6aaaaajQQQQQQQQQQ\n\n");
    printf("    Have you seen the marvelous Breadfish\n");
    printf("  Swimming in the ocean water!!!\n");
    printf("=====================================================\n\n\n");

}