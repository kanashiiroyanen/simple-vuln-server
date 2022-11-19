 言語プログラムに対する exploit のデモ
 C 言語で作成した TCP サーバに対して、バッファオーバフロー攻撃を実施する。
 exploit が成功した場合、reverse shell が起動するため、netcat 等で待ち受ける。


# Demo（通常の使い方）
1. vuln_server を動作させる。
　・$ ./vuln_server

2. 別ターミナルで netcat を実行してサーバに接続し、好きな文字列を入力する。
　・$ nc -v localhost 8888

3. finish とうち、次に適当な文字列を入力するとサーバプログラムが終了する。


# Demo（exploit）
1. vuln_server を動作させる。
　・$ ./vuln_server

2. 別ターミナルで netcat を動作させる。
　・$ nc -v -l 4444

3. 別ターミナルで python の exploit を実行する。アドレスはサーバプログラムで表示されたものを指定する。バッファサイズは vuln_server の buf サイズを指定する。
　・$ python exploit-reverse.py 0x7fffffffe200 200


# 事前準備
1. git clone でファイルダウンロード

2. ASLR を無効化する。
　sudo sysctl -w kernel.randomize_va_space=0

3. gcc でコンパイルする。
　gcc -fno-stack-protector -z execstack vuln_server.c -o vuln_server


# デバッグ方法
1. gdb -q vuln_server
デバッグ方法
1. gdb -q vuln_server

2. disas main

3. b *0xfxxxxxxxx
  setting breakpoint 

4. run
  run this program

5. i r $rsp $rbp
  after break, check to their resisters.

6. x/60gx $rsp
  dumping stack and check return address.

