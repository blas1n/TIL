open System

let rec sum now =
    match now with
    |1 -> 1
    |_ -> now + sum (now - 1)

let rec multi now =
    match now with
    |1 -> 1
    |_ -> now * multi (now - 1)

[<EntryPoint>]
let main argv =
    let mutable bLoop = true

    while bLoop do
        Console.Write "명령어와 숫자를 입력하세요 (ex. Sum 100). "
        let input = Console.ReadLine ()
        let inputSplit = input.Split ' '
        
        let setOperator (str : string[]) =
            match str.Length with
            |1 -> 0
            |_ -> int str.[1]

        let operator = inputSplit.[0].ToUpper ()
        let operand = setOperator inputSplit

        match operator with
        |"SUM" -> printfn "%d" (sum operand)
        |"MULTI" -> printfn "%d" (multi operand)
        |"EXIT" -> bLoop <- false
        |_ -> printfn "잘 못된 입력"

        Console.WriteLine ()

    Console.WriteLine "종료"
    0