{
    var sum = 0;
    var i = 0;
    while (i < 400) {
        var j = 0;
        while (j < 400) {
            sum = sum + i + j;
            j = j + 1;
        }
        i = i + 1;
    }
    print(sum);
}
