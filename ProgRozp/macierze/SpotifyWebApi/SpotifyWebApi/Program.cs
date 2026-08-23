using SpotifyWebApi;
using System.Diagnostics;
using System.Net;
using System.Net.Http.Headers;
using System.Text;
using System.Text.Json;

internal class Program
{
    static async Task Main()
    {
        string token = await GetUserAccessToken();
        while (true)
        {
            Console.WriteLine("\n==Spotify WEB API ===");
            Console.WriteLine("1. Wyszukaj utwór");
            Console.WriteLine("2. Zobacz playlisty");
            Console.WriteLine("3. Stworz playliste");
            Console.WriteLine("4. Dodaj utwor do playlisty");
            Console.WriteLine("5. Zmien nazwe playlisty");
            Console.WriteLine("6. Usun utwor z playlisty");
            Console.WriteLine("0.Wyjscie");
            Console.Write("Wybierz opcję: ");
            
            string? option = Console.ReadLine();
            switch (option)
            {
                case "1":
                    await SearchTrack(token);
                    break;
                case "2":
                    await SearchPlaylist(token);
                    break;
                case "3":
                    await CreatePlaylist(token);
                    break;
                case "4":
                    await AddSongToPlaylist(token);
                    break;
                case "5":
                    await ChangeNamePlaylist(token);
                    break;
                case "6":
                    await DeleteSongFromPlaylist(token);
                    break;
                case "0":
                    return;
                default:
                    Console.WriteLine("Nieprawidłowa opcja.");
                    break;
            }
        }
        
    }
    
   static async Task<string> GetUserAccessToken()
    {
        string scopes = "user-read-private playlist-modify-public playlist-modify-private playlist-read-private";
        string authUrl =
            "https://accounts.spotify.com/authorize" +
            $"?client_id={SpotifyConfig.ClientId}" +
            "&response_type=code" +
            $"&redirect_uri={Uri.EscapeDataString(SpotifyConfig.RedirectUri)}" +
            $"&scope={Uri.EscapeDataString(scopes)}" +
            "&show_dialog=true";

        using var listener = new HttpListener();
        listener.Prefixes.Add(SpotifyConfig.RedirectUri + "/");
        listener.Start();
        Process.Start(new ProcessStartInfo{ FileName = authUrl, UseShellExecute = true });

        Console.WriteLine("Zaloguj się w przeglądarce do Spotify...");

        var context = await listener.GetContextAsync();

        string? code = context.Request.QueryString["code"];

        string responseText = "Możesz zamknąć tę kartę i wrócić do aplikacji.";

        byte[] buffer = Encoding.UTF8.GetBytes(responseText);

        context.Response.ContentLength64 = buffer.Length;

        await context.Response.OutputStream.WriteAsync(buffer);

        context.Response.Close();

        listener.Stop();

        if (string.IsNullOrWhiteSpace(code))
            throw new Exception("Nie udało się pobrać kodu autoryzacyjnego.");
        
        using var client = new HttpClient();
        string auth = Convert.ToBase64String(Encoding.UTF8.GetBytes($"{SpotifyConfig.ClientId}:{SpotifyConfig.ClientSecret}"));
        
        client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic", auth);

        var tokenResponse = await client.PostAsync(
            "https://accounts.spotify.com/api/token",
            new FormUrlEncodedContent(new Dictionary<string, string>
            {
                { "grant_type", "authorization_code" },
                { "code", code },
                { "redirect_uri", SpotifyConfig.RedirectUri }
            }));

        string json = await tokenResponse.Content.ReadAsStringAsync();
        if (!tokenResponse.IsSuccessStatusCode)
        {
            Console.WriteLine(json);
            throw new Exception("Błąd pobierania tokenu użytkownika.");
        }
        using var document = JsonDocument.Parse(json);
        return document.RootElement.GetProperty("access_token").GetString()!;

    }

    static async Task SearchTrack(string token)
    {
        Console.Write("Podaj nazwe utworu: ");
        string? query = Console.ReadLine();
        if (string.IsNullOrWhiteSpace(query))
        {
            Console.WriteLine("Nie podano nazwy utworu.");
            return;
        }
        using var client = new HttpClient();
        client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);
        string url = $"https://api.spotify.com/v1/search?q={Uri.EscapeDataString(query)}&type=track&limit=5";
        var response = await client.GetAsync(url);
        var json = await response.Content.ReadAsStringAsync();

        if (!response.IsSuccessStatusCode)
        {
            Console.WriteLine("Spotify zwróciło błąd:");
            Console.WriteLine(await response.Content.ReadAsStringAsync());
            return;
        }

        using var document = JsonDocument.Parse(json);

        if (!document.RootElement.TryGetProperty("tracks", out var tracksObject))
        {
            Console.WriteLine("Brak pola tracks w odpowiedzi Spotify.");
            return;
        }

        var tracks = tracksObject.GetProperty("items");
        

        foreach (var track in tracks.EnumerateArray())
        {
            string name = track.GetProperty("name").GetString()!;
            string album = track.GetProperty("album").GetProperty("name").GetString()!;
            string releaseDate = track.GetProperty("album").GetProperty("release_date").GetString()!;

            string artist = track.GetProperty("artists")[0].GetProperty("name").GetString()!;
            string uri = track.GetProperty("uri").GetString()!;
            Console.WriteLine($"\nUtwór: {name}");
            Console.WriteLine($"Artysta: {artist}");
            Console.WriteLine($"Album: {album}");
            Console.WriteLine($"Data wydania: {releaseDate}");
            Console.WriteLine($"URI: {uri}");
        }
    }
    static async Task SearchPlaylist(string token)
    {
        
        using var client = new HttpClient();
        client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);
        string url = $"https://api.spotify.com/v1/me/playlists";
        var response = await client.GetAsync(url);
        var json = await response.Content.ReadAsStringAsync();

        if (!response.IsSuccessStatusCode)
        {
            Console.WriteLine("Spotify zwróciło błąd:");
            Console.WriteLine(await response.Content.ReadAsStringAsync());
            return;
        }

        using var document = JsonDocument.Parse(json);

        
        var playlists = document.RootElement.GetProperty("items");
        

        foreach (var playlist in playlists.EnumerateArray())
        {
            string name = playlist.GetProperty("name").GetString()!;
            string id = playlist.GetProperty("id").GetString()!;
            string owner = playlist.GetProperty("owner").GetProperty("id").GetString()!;
        
            Console.WriteLine($"\nPlaylista: {name}");
            Console.WriteLine($"Id: {id}");
            Console.WriteLine($"Owner ID: {owner}");
        }
    }
    static async Task<string> GetCurrentUserId(string token)
    {
        using var client = new HttpClient();

        client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);

        var response = await client.GetAsync("https://api.spotify.com/v1/me");

        string json = await response.Content.ReadAsStringAsync();

        // Console.WriteLine("CURRENT USER JSON:");
        // Console.WriteLine(json);

        using var document = JsonDocument.Parse(json);

        return document.RootElement.GetProperty("id").GetString()!;
    }
    static async Task CreatePlaylist(string token)
    {
        Console.Write("Podaj nazwe playlisty: ");
        string? Playlistname = Console.ReadLine();
        if (string.IsNullOrWhiteSpace(Playlistname))
        {
            Console.WriteLine("Nie podano nazwy playlisty.");
            return;
        }
        using var client = new HttpClient();
        client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);
        string url = "https://api.spotify.com/v1/me/playlists";
        var body = new
        {
            name = Playlistname,
            description = "Playlista o nazwie " + Playlistname,
            @public = false
        };
        string jsonBody = JsonSerializer.Serialize(body);
        Console.WriteLine("CREATE PLAYLIST URL:");
        Console.WriteLine(url);
        Console.WriteLine("CREATE PLAYLIST BODY:");
        Console.WriteLine(jsonBody);
        var response = await client.PostAsync(url, new StringContent(jsonBody, Encoding.UTF8, "application/json"));
        if (!response.IsSuccessStatusCode)
        {
            Console.WriteLine("Spotify zwróciło błąd:");
            Console.WriteLine(await response.Content.ReadAsStringAsync());
            return;
        }
        Console.WriteLine("Playlista dodana pomyslnie");
    }
    
    static async Task AddSongToPlaylist(string token)
    {
        Console.Write("Podaj id playlisty: ");
        string? PlaylistId = Console.ReadLine();
        if (string.IsNullOrWhiteSpace(PlaylistId))
        {
            Console.WriteLine("Nie podano id.");
            return;
        }
        Console.Write("Podaj uri piosenki: ");
        string? SongUri = Console.ReadLine();
        if (string.IsNullOrWhiteSpace(SongUri))
        {
            Console.WriteLine("Nie podano uri.");
            return;
        }
        PlaylistId = PlaylistId.Trim();
        SongUri = SongUri.Trim();

        if (!SongUri.StartsWith("spotify:track:"))
        {
            Console.WriteLine("URI utworu powinno zaczynać się od spotify:track:");
            return;
        }
        using var client = new HttpClient();
        client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);
        string url = $"https://api.spotify.com/v1/playlists/{PlaylistId}/items";
        var body = new { uris = new[] {SongUri} };
        string jsonBody = JsonSerializer.Serialize(body);
        var response = await client.PostAsync(url, new StringContent(jsonBody, Encoding.UTF8, "application/json"));
        string responseJson = await response.Content.ReadAsStringAsync();

        if (!response.IsSuccessStatusCode)
        {
            Console.WriteLine("Spotify zwróciło błąd:");
            Console.WriteLine(responseJson);
            return;
        }

        Console.WriteLine("Dodano piosenke");
        // Console.WriteLine(responseJson);
    }
    static async Task ChangeNamePlaylist(string token)
    {
        Console.Write("Podaj Id playlisty: ");
        string? PlaylistId = Console.ReadLine();
        if (string.IsNullOrWhiteSpace(PlaylistId))
        {
            Console.WriteLine("Nie podano id playlisty.");
            return;
        }
        Console.Write("Podaj nowa nazwe playlisty: ");
        string? Playlistname = Console.ReadLine();
        if (string.IsNullOrWhiteSpace(Playlistname))
        {
            Console.WriteLine("Nie podano nazwy playlisty.");
            return;
        }
        using var client = new HttpClient();
        client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);
        string url = $"https://api.spotify.com/v1/playlists/{PlaylistId}";
        var body = new
        {
            name = Playlistname,
            description = "Playlista o nazwie " + Playlistname,
            @public = true
        };
        string jsonBody = JsonSerializer.Serialize(body);
        var response = await client.PutAsync(url, new StringContent(jsonBody, Encoding.UTF8, "application/json"));
        if (!response.IsSuccessStatusCode)
        {
            Console.WriteLine("Spotify zwróciło błąd:");
            Console.WriteLine(await response.Content.ReadAsStringAsync());
            return;
        }
        Console.WriteLine("Zmieniono nazwe playlisty");
    }
    static async Task DeleteSongFromPlaylist(string token)
    {
        Console.Write("Podaj id playlisty: ");
        string? PlaylistId = Console.ReadLine();
        if (string.IsNullOrWhiteSpace(PlaylistId))
        {
            Console.WriteLine("Nie podano id.");
            return;
        }
        Console.Write("Podaj uri piosenki: ");
        string? SongUri = Console.ReadLine();
        if (string.IsNullOrWhiteSpace(SongUri))
        {
            Console.WriteLine("Nie podano uri.");
            return;
        }
        PlaylistId = PlaylistId.Trim();
        SongUri = SongUri.Trim();

        if (!SongUri.StartsWith("spotify:track:"))
        {
            Console.WriteLine("URI utworu powinno zaczynać się od spotify:track:");
            return;
        }
        using var client = new HttpClient();
        client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);
        
        var body = new
        {
            items = new[]
            {
                new { uri = SongUri }
            }
        };
        string jsonBody = JsonSerializer.Serialize(body);
        Console.WriteLine("DELETE TRACK URL:");
        Console.WriteLine($"https://api.spotify.com/v1/playlists/{PlaylistId}/items");
        Console.WriteLine("DELETE TRACK BODY:");
        Console.WriteLine(jsonBody);
        var request = new HttpRequestMessage(
            HttpMethod.Delete,
            $"https://api.spotify.com/v1/playlists/{PlaylistId}/items");

        request.Content = new StringContent(jsonBody, Encoding.UTF8, "application/json");
        var response = await client.SendAsync(request);
        string responseJson = await response.Content.ReadAsStringAsync();
        if (!response.IsSuccessStatusCode)
        {
            Console.WriteLine("Spotify zwróciło błąd:");
            Console.WriteLine(responseJson);
            return;
        }
        Console.WriteLine("Usunieto piosenke");
        Console.WriteLine(responseJson);
    }
}