Imports System
Imports System.IO

Module Program

    Private Structure Claim

        Public id As Integer
        Public x As Integer
        Public y As Integer
        Public width As Integer
        Public height As Integer

    End Structure

    Function Atoi(ByRef line As String, ByRef index As Integer) As Integer

        Dim id As Integer
        id = 0

        While index < line.Length

            If Not (Convert.ToInt32(line.ElementAt(index)) >= Convert.ToInt32("0"c) And
                    Convert.ToInt32(line.ElementAt(index)) <= Convert.ToInt32("9"c)) Then

                Return id

            End If

            id = (10 * id)
            id = id + (Convert.ToInt32(line.ElementAt(index)) - Convert.ToInt32("0"c))

            index = index + 1

        End While

        Return id

    End Function


    Sub Main(args As String())

        Dim inputData As String
        Dim index As Integer

        Dim matrixWidth As Integer
        Dim matrixHeight As Integer

        Dim result1 As Integer
        Dim result2 As Integer

        matrixWidth = 0
        matrixHeight = 0

        result1 = 0
        result2 = 0

        Dim claims As New ArrayList

        index = 0

        inputData = File.ReadAllText("claims.in")

        While index < inputData.Length

            If inputData.ElementAt(index) = "#" Then

                Dim props As Claim

                Dim id As Integer
                Dim x As Integer
                Dim y As Integer
                Dim width As Integer
                Dim height As Integer

                id = 0
                x = 0
                y = 0
                width = 0
                height = 0

                index = index + 1

                id = Atoi(inputData, index)

                props.id = id

                index = index + 1
                index = index + 1
                index = index + 1

                x = Atoi(inputData, index)

                index = index + 1

                y = Atoi(inputData, index)

                props.x = x
                props.y = y

                index = index + 1
                index = index + 1

                width = Atoi(inputData, index)

                index = index + 1

                height = Atoi(inputData, index)

                props.width = width
                props.height = height

                If x + width > matrixWidth Then
                    matrixWidth = x + width
                End If

                If y + height > matrixHeight Then
                    matrixHeight = y + height
                End If

                claims.Add(props)

            End If

            index = index + 1

        End While

        Dim matrix(matrixHeight, matrixWidth) As Integer

        For i = 0 To matrixHeight - 1

            For j = 0 To matrixWidth - 1

                matrix(i, j) = 0

            Next

        Next

        For Each prop As Claim In claims

            For i = prop.y To prop.y + prop.height - 1

                For j = prop.x To prop.x + prop.width - 1

                    matrix(i, j) = matrix(i, j) + 1

                Next

            Next

        Next

        For Each prop As Claim In claims

            Dim overlap As Boolean
            overlap = False

            For i = prop.y To prop.y + prop.height - 1

                For j = prop.x To prop.x + prop.width - 1

                    If matrix(i, j) >= 2 Then
                        overlap = True
                    End If

                Next

            Next

            If Not (overlap) Then

                result2 = prop.id

            End If
        Next

        For i = 0 To matrixHeight - 1

            For j = 0 To matrixWidth - 1

                If matrix(i, j) >= 2 Then

                    result1 = result1 + 1

                End If

            Next

        Next

        File.WriteAllText("claims.out", String.Format("{0}\n{1}", result1, result2).Replace("\n", Environment.NewLine))

    End Sub

End Module
