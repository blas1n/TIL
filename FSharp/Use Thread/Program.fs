open System
open System.Threading

let ThreadA() =
    while(true) do
        printfn "[A] Timer tick %A" DateTime.Now
        Thread.Sleep 1000

let ThreadB() =
    while(true) do
        printfn "[B] Timer tick %A" DateTime.Now
        Thread.Sleep 1000

[<EntryPoint>]
let main argv =
    let thread_a = new Thread(new ThreadStart(fun _ -> ThreadA()))
    thread_a.IsBackground <- true
    thread_a.Start()

    let thread_b = new Thread(new ThreadStart(fun _ -> ThreadB()))
    thread_b.IsBackground <- true
    thread_b.Start()

    Console.ReadKey() |> ignore
    0