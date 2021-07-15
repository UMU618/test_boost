#include <iostream>
#include <array>

#include <boost/io/ostream_joiner.hpp>

int main() {
  std::array<int,6> a{1, 2, 3, 4, 5, 6};
  std::copy(a.begin(), a.end(), boost::io::make_ostream_joiner(std::cout, ','));
}

/*
������ܵ���ÿ�������м��һ�� ','��ͷβ����û�� ',',
��c����ʵ�� ����if
���磺
���룺a[6] = {1, 2, 3, 4, 5, 6}
�����1,2,3,4,5,6

#include <stdio.h>
int main(void) {
    int a[6] = {1, 2, 3, 4, 5, 6}, i;
    for (i = 0; i < 6; i++) {
        printf(",%d" + !i, a[i]);
    }
    return 0;
}
*/