# union work todo

    union Foo
      a::Sint
      b::String
    end

    fn main()
      let f = Foo("Hello")

      match f
        case a::Sint
          println(a)
        case b::String
          println(b)
        end
      end
    end

