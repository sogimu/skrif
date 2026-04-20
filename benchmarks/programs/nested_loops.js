{
    var sum = 0;
    var i = 0;
    while (i < 60) {
        var j = 0;
        while (j < 60) {
            sum = sum + i + j;
            j = j + 1;
        }
        i = i + 1;
    }
    print(sum);
}
