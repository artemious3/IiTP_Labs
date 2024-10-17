
namespace Padhaiski.Domain;
public class Hospital : IEquatable<Hospital>
{
    public List<Patient> WaitingList;

    public Hospital()
    {
        WaitingList = new List<Patient>();
    }

    public override string ToString()
    {
        return string.Concat(
            from p in WaitingList
            select p.ToString()+'\n'
        );
    }

    public bool Equals(Hospital? er) 
    {
        if(er.WaitingList.Count != this.WaitingList.Count)
            return false;

        for(int i = 0; i < er.WaitingList.Count; ++i)
        {
            if(er.WaitingList[i] != WaitingList[i]){
                return false;
            }
        }
        return true;
    }

}