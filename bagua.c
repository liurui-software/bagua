#include <stdio.h>
#include <string.h>
#include <time.h>


#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <conio.h>
  typedef struct {
      LARGE_INTEGER start;
  } Timer;
#else
  #include <unistd.h>
  #include <termios.h>     // 提供终端 I/O 控制结构体
  typedef struct {
      struct timespec start;
  } Timer;
#endif

#define LR_BG_VER "3.0"


/**
 */
void wait(long ms) {
#ifdef _WIN32
  Sleep(ms);
#else
  sleep(ms/1000.);
#endif
}


/**
 * 开始计时，返回一个 Timer 对象
 */
Timer timer_start(void) {
  Timer t;
#ifdef _WIN32
  QueryPerformanceCounter(&t.start);
#else
  clock_gettime(CLOCK_MONOTONIC, &t.start);
#endif
  return t;
}


/**
 * 计算从 start 时刻到当前时刻经过的毫秒数
 */
long timer_elapsed_ms(Timer start) {
#ifdef _WIN32
  LARGE_INTEGER now, freq;
  QueryPerformanceCounter(&now);
  QueryPerformanceFrequency(&freq);
  // 计算差值（纳秒级），再转为毫秒
  // (now - start) * 1000 / freq
  return (now.QuadPart - start.start.QuadPart) * 1000 / freq.QuadPart;
#else
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  // 计算秒和纳秒的总差值
  long long diff_sec = now.tv_sec - start.start.tv_sec;
  long long diff_nsec = now.tv_nsec - start.start.tv_nsec;
  // 转为毫秒
  return diff_sec * 1000 + diff_nsec / 1000000;
#endif
}


/**
 * 跨平台等待并接受用户按下的任意键，不显示输入。
 */
void wait_for_any_key() {
#ifdef _WIN32
  _getch();  // Windows 原生实现，无需回车[reference:5]
#else
  struct termios oldt, newt;

  // 获取当前终端的属性，并备份到 oldt
  tcgetattr(STDIN_FILENO, &oldt);
  
  // 基于旧设置创建新设置
  newt = oldt;
  
  // 清除规范模式 (ICANON) 和回显 (ECHO) 标志位
  newt.c_lflag &= ~(ICANON | ECHO);
  
  // 立即应用新设置
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  
  // 读取一个字符，程序将在此处等待
  getchar();
  
  // 恢复原来的终端设置
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}


char *Trigrams[] = {
"乾上乾下 - 乾卦",
"坤上坤下 - 坤卦",
"坎上震下 - 屯卦",
"艮上坎下 - 蒙卦",
"坎上乾下 - 需卦",
"乾上坎下 - 讼卦",
"坤上坎下 - 师卦",
"坎上坤下 - 比卦",
"巽上乾下 - 小畜卦",
"乾上兑下 - 履卦",
"坤上乾下 - 泰卦",
"乾上坤下 - 否卦",
"乾上离下 - 同人卦",
"离上乾下 - 大有卦",
"坤上艮下 - 谦卦",
"震上坤下 - 豫卦",
"兑上震下 - 随卦",
"艮上巽下 - 蛊卦",
"坤上兑下 - 临卦",
"巽上坤下 - 观卦",
"离上震下 - 噬嗑卦",
"艮上离下 - 贲卦",
"艮上坤下 - 剥卦",
"坤上震下 - 复卦",
"乾上震下 - 无妄卦",
"艮上乾下 - 大畜卦",
"艮上震下 - 颐卦",
"兑上巽下 - 大过卦",
"坎上坎下 - 坎卦",
"离上离下 - 离卦",
"兑上艮下 - 咸卦",
"震上巽下 - 恒卦",
"乾上艮下 - 遁卦",
"震上乾下 - 大壮卦",
"离上坤下 - 晋卦",
"坤上离下 - 明夷卦",
"巽上离下 - 家人卦",
"离上兑下 - 睽卦",
"坎上艮下 - 蹇卦",
"震上坎下 - 解卦",
"艮上兑下 - 损卦",
"巽上震下 - 益卦",
"兑上乾下 - 夬卦",
"乾上巽下 - 姤卦",
"兑上坤下 - 萃卦",
"坤上巽下 - 升卦",
"坎上兑下 - 困卦",
"巽上坎下 - 井卦",
"离上兑下 - 革卦",
"离上巽下 - 鼎卦",
"震上震下 - 震卦",
"艮上艮下 - 艮卦",
"巽上艮下 - 渐卦",
"震上兑下 - 归妹卦",
"震上离下 - 丰卦",
"离上艮下 - 旅卦",
"巽上巽下 - 巽卦",
"兑上兑下 - 兑卦",
"巽上坎下 - 涣卦",
"坎上兑下 - 节卦",
"巽上兑下 - 中孚卦",
"震上艮下 - 小过卦",
"坎上离下 - 既济卦",
"离上坎下 - 未济卦"};


/*
  */
void BgName(char *bg, char *g){
  if( !memcmp(bg, "\x1\x1\x1", 3) )
    strcpy(g, "坤");
  else if( !memcmp(bg, "\x0\x1\x1", 3) )
    strcpy(g, "震");
  else if( !memcmp(bg, "\x1\x0\x1", 3) )
    strcpy(g, "坎");
  else if( !memcmp(bg, "\x0\x0\x1", 3) )
    strcpy(g, "兑");
  else if( !memcmp(bg, "\x1\x1\x0", 3) )
    strcpy(g, "艮");
  else if( !memcmp(bg, "\x0\x1\x0", 3) )
    strcpy(g, "离");
  else if( !memcmp(bg, "\x1\x0\x0", 3) )
    strcpy(g, "巽");
  else
    strcpy(g, "乾");
}


/*
  */
void BgStr(char *bg, char *bgs){
  char upper[10], lower[10];
  int len0, i;
  
  BgName(bg, lower);
  BgName(bg+3, upper);

  sprintf( bgs, "%s上%s下", upper, lower ); 
  len0= strlen(bgs);
  for(i=0; i<64; i++){
    if( !strncmp(Trigrams[i], bgs, len0) ){
      strcpy(bgs, Trigrams[i]);
      break;
    }
  }
}


/*
  */
int main( void ){
#ifdef _WIN32
  SetConsoleOutputCP(65001);
#endif
  
  long seq, t;
  int i, change;
  char bg0[6], bg1[6], bg0s[50], bg1s[50];

  printf("----------------------------\nLR八卦生成器 (版本%s)\n----------------------------\n\n", LR_BG_VER);

  for(i=0, seq=0; i<6; i++){
    printf( "请按键：第%d爻!\n", (1+i) );
    Timer t0= timer_start();
    wait_for_any_key();
    t= timer_elapsed_ms(t0);
    bg0[i] = t%2;
    seq += t;
  }
  change=seq%7;
  //printf( "Inputed: %d,%d,%d; %d,%d,%d. (%d)\n", bg0[0],bg0[1],bg0[2],bg0[3],bg0[4],bg0[5], change );

  for(i=0; i<6; i++)
    bg1[i]= bg0[i];

  if(change)
    bg1[change-1]= bg0[change-1] ^ 1;

  //printf( "Targeted: %d,%d,%d; %d,%d,%d.\n", bg1[0],bg1[1],bg1[2],bg1[3],bg1[4],bg1[5] );

  BgStr(bg0, bg0s);
  BgStr(bg1, bg1s);

  printf( " %s\n %s\n (变%d)\n", bg0s, bg1s, change );
  return 0;
}
