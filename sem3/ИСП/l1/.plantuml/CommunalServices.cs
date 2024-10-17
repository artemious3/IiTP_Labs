using System.Numerics;

record struct CommunalServices(int Gas, int Water, int Energy)
 : IAdditionOperators<CommunalServices, CommunalServices, CommunalServices>,
   IMultiplyOperators<CommunalServices, CommunalServices, int>
{

    static public CommunalServices operator +(CommunalServices left, CommunalServices right)
    {
        return new(left.Gas + right.Gas, left.Water + right.Water, left.Energy + right.Energy);
    }
    static public int operator *(CommunalServices left, CommunalServices right)
    {
        return left.Gas * right.Gas + left.Water * right.Water + left.Energy * right.Energy;
    }
}



